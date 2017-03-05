#include <vector>
#include <memory>
#include <Windows.h>
#include <algorithm>
#include <fstream>
#include <ctime>

#include "Object.h"
#include "Light.h"
#include "Options.h"
#include "Math.h"
#include "Sphere.h"
#include "MeshTriangle.h"

int g_totalRays = 0;

bool trace(const Vec3f& orig, const Vec3f& dir,
    const std::vector<std::unique_ptr<Object>>& objects,
    float& tNear, uint32_t& index, Vec2f& uv, Object** hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = INFINITY;
        uint32_t indexK;
        Vec2f uvK;
        if (objects[k]->intersect(orig, dir, tNearK, indexK, uvK) &&
            tNearK < tNear) {
            *hitObject = objects[k].get();
            tNear = tNearK;
            index = indexK;
            uv = uvK;
        }
    }

    return (*hitObject != nullptr);
}

Vec3f castRay(const Vec3f& orig, const Vec3f& dir,
    const std::vector<std::unique_ptr<Object>>& objects,
    const std::vector<std::unique_ptr<Light>>& lights,
    const Options& options,
    uint32_t depth, bool test = false)
{
    g_totalRays++;

    if (depth > options.maxDepth) {
        return options.backgroundColour;
    }

    Vec3f hitColour = options.backgroundColour;
    float tNear = INFINITY;
    Vec2f uv;
    uint32_t index = 0;
    Object* hitObject = nullptr;

    if (trace(orig, dir, objects, tNear, index, uv, &hitObject)) {
        Vec3f hitPoint = orig + dir * tNear;
        Vec3f N;
        Vec2f st;
        hitObject->getSurfaceProperties(hitPoint, dir, index, uv, N, st);
        Vec3f tmp = hitPoint;

        switch (hitObject->materialType) {
        case REFLECTION_AND_REFRACTION:
        {
            Vec3f reflectionDirection = Math::reflect(dir, N);
            reflectionDirection.normalize();
            Vec3f refractionDirection = Math::refract(dir, N, hitObject->ior);
            refractionDirection.normalize();
            Vec3f reflectionRayOrig = (reflectionDirection.dotProduct(N) < 0) ?
                hitPoint - N * options.bias :
                hitPoint + N * options.bias;
            Vec3f refractionRayOrig = (refractionDirection.dotProduct(N) < 0) ?
                hitPoint - N * options.bias :
                hitPoint + N * options.bias;
            Vec3f reflectionColour = castRay(reflectionRayOrig,
                reflectionDirection, objects, lights, options, depth + 1, 1);
            Vec3f refractionColour = castRay(refractionRayOrig,
                refractionDirection, objects, lights, options, depth + 1, 1);
            float kr;
            Math::fresnel(dir, N, hitObject->ior, kr);
            hitColour = reflectionColour * kr + refractionColour * (1 - kr);

            break;
        }

        case REFLECTION:
        {
            float kr;
            Math::fresnel(dir, N, hitObject->ior, kr);
            Vec3f reflectionDirection = Math::reflect(dir, N);
            Vec3f reflectionRayOrig = (reflectionDirection.dotProduct(N) < 0) ?
                hitPoint + N * options.bias :
                hitPoint - N * options.bias;

            hitColour = castRay(reflectionRayOrig, reflectionDirection,
                objects, lights, options, depth + 1) * kr;

            break;
        }

        default:
        {
            Vec3f lightAmt = 0, specularColour = 0;
            Vec3f shadowPointOrig = (dir.dotProduct(N) < 0) ?
                hitPoint + N * options.bias :
                hitPoint - N * options.bias;

            for (uint32_t i = 0; i < lights.size(); ++i) {
                Vec3f lightDir = lights[i]->position - hitPoint;
                float lightDistance2 = lightDir.dotProduct(lightDir);
                lightDir.normalize();
                float LdotN = max(0.f, lightDir.dotProduct(N));
                Object* shadowHitObject = nullptr;
                float tNearShadow = INFINITY;

                bool inShadow = trace(shadowPointOrig, lightDir, objects,
                    tNearShadow, index, uv, &shadowHitObject) &&
                    tNearShadow * tNearShadow < lightDistance2;
                lightAmt += (1.0f - inShadow) * lights[i]->intensity * LdotN;
                Vec3f reflectionDirection = Math::reflect(-lightDir, N);
                specularColour += powf(max(0.f,
                    -reflectionDirection.dotProduct(dir)),
                    hitObject->specularExponent) * lights[i]->intensity;
            }
            hitColour = lightAmt * hitObject->evalDiffuseColour(st) *
                hitObject->Kd + specularColour * hitObject->Ks;

            break;
        }
        }
    }

    return hitColour;
}

void render(const Options& options,
    const std::vector<std::unique_ptr<Object>>& objects,
    const std::vector<std::unique_ptr<Light>>& lights)
{
    Vec3f* framebuffer = new Vec3f[options.width * options.height];
    Vec3f* pix = framebuffer;
    float scale = tan(Math::deg2rad(options.fov * 0.5f));
    float imageAspectRatio = options.width / (float)options.height;
    Vec3f orig(0);

    for (uint32_t j = 0; j < options.height; ++j) {
        for (uint32_t i = 0; i < options.width; ++i) {
            // primary ray
            float x = (2.0f * (i + 0.5f) / (float)options.width - 1.0f) *
                imageAspectRatio * scale;
            float y = (1.0f - 2.0f * (j + 0.5f) / (float)options.height) * scale;
            Vec3f dir = Vec3f(x, y, -1);
            dir.normalize();
            *(pix++) = castRay(orig, dir, objects, lights, options, 0);
        }
    }

    // save framebuffer to file
    std::ofstream ofs;
    ofs.open("out.ppm", std::ios_base::out | std::ios_base::binary);

    ofs << "P6\n" << options.width << " " << options.height << "\n255\n";
    for (uint32_t i = 0; i < options.height * options.width; ++i) {
        char r = (char)(255 * Math::clamp(0, 1, framebuffer[i].x));
        char g = (char)(255 * Math::clamp(0, 1, framebuffer[i].y));
        char b = (char)(255 * Math::clamp(0, 1, framebuffer[i].z));
        ofs << r << g << b;
    }
    ofs.close();

    delete[] framebuffer;
}

int main()
{
    std::vector<std::unique_ptr<Object>> objects;
    std::vector<std::unique_ptr<Light>> lights;

    Sphere* sph1 = new Sphere(Vec3f(3.0f, 0.0f, -12.0f), 2.0f);
    sph1->materialType = DIFFUSE_AND_GLOSSY;
    sph1->diffuseColour = Vec3f(0.6f, 0.7f, 0.8f);

    Sphere* sph2 = new Sphere(Vec3f(0.5, -0.5, -8), 1.5);
    sph2->ior = 1.5;
    sph2->materialType = REFLECTION_AND_REFRACTION;

    objects.push_back(std::unique_ptr<Sphere>(sph1));
    objects.push_back(std::unique_ptr<Sphere>(sph2));

    Vec3f verts[4] = { {-5, -3, -6}, {5, -3, -6}, {5, -3, -16}, {-5, -3, -16} };
    uint32_t vertIndex[6] = { 0, 1, 3, 1, 2, 3 };
    Vec2f st[4] = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };
    MeshTriangle* mesh = new MeshTriangle(verts, vertIndex, 2, st);
    mesh->materialType = DIFFUSE_AND_GLOSSY;

    objects.push_back(std::unique_ptr<MeshTriangle>(mesh));

    lights.push_back(std::unique_ptr<Light>(new Light(Vec3f(-20, 70, 20), 0.5)));
    lights.push_back(std::unique_ptr<Light>(new Light(Vec3f(30, 50, -12), 1)));

    Options options;
    options.width = 4000;
    options.height = 3000;
    options.fov = 90;
    options.backgroundColour = Vec3f(0.2f, 0.4f, 0.6f);
    options.maxDepth = 5;
    options.bias = 0.00001f;

    std::cout << "Starting render..." << std::endl;
    std::clock_t start;
    start = std::clock();
    render(options, objects, lights);
    std::cout << "Render finished: " <<
        (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    std::cout << "Total rays: " << g_totalRays << std::endl;

    return 0;
}