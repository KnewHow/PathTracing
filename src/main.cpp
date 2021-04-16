#include <iostream>
#include <memory>
#include <atomic>
#include <thread>
#include <chrono>

#include "Vector.hpp"
#include "Image.hpp"
#include "common/Tools.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Renderer.hpp"
#include "geometry/Sphere.hpp"
#include "material/Material.hpp"
#include "material/Diffuse.hpp"
#include "material/Metal.hpp"
#include "material/Dielectric.hpp"

void random_scene(Scene& scene) {
    for(int a = -11; a < 11; a++) {
        for(int b = -11; b < 11; b++) {
            float choose_mat = get_random_float();
            tinyMath::vec3f center =  tinyMath::vec3f(a + 0.9 * get_random_float(), 0.2f, b + 0.9 * get_random_float());
            if((center - tinyMath::vec3f(4.0, 0.2, 0)).norm() > 0.9) {
                std::shared_ptr<Material> shpere_mat;
                if(choose_mat < 0.8) { // diffuse 
                    tinyMath::vec3f albedo = get_random_vector() * get_random_vector();
                    shpere_mat = std::make_shared<Diffuse>(albedo);
                } else if(choose_mat < 0.95) { // metal
                    tinyMath::vec3f color = get_random_vector(0.5f, 1.0f);
                    float fuzz = get_random_float(0.0f, 0.5f);
                    shpere_mat = std::make_shared<Metal>(color, fuzz);
                } else { // glass m
                    shpere_mat = std::make_shared<Dielectric>(1.5f);
                }
                scene.addObject(std::make_shared<Sphere>(center, 0.2f, shpere_mat));
            }
        }
    }
}

int main() {
    int width = 1200;
    float aspect_ratio = 3.0f / 2.0f;
    std::string filepath = "./result.ppm";
    std::shared_ptr<Image> image = std::make_shared<Image>(width, aspect_ratio, filepath);
    tinyMath::vec3f lookfrom = tinyMath::vec3f(13.0f, 2.0f, 3.0f);
    tinyMath::vec3f lookat = tinyMath::vec3f(0.0f, 0.0f, 0.0f);
    tinyMath::vec3f vup = tinyMath::vec3f(0.0f, 1.0f, 0.0f);
    float focus_dist = 10.0f;
    float aperture = 0.1f;
    Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, focus_dist);
    
    std::shared_ptr<Material> mat_ground = std::make_shared<Diffuse>(tinyMath::vec3f(0.5f, 0.5f, 0.5f));
    std::shared_ptr<Material> mat_1 = std::make_shared<Dielectric>(1.5f);
    std::shared_ptr<Material> mat_2 = std::make_shared<Diffuse>(tinyMath::vec3f(0.4f, 0.2f, 0.1f));
    std::shared_ptr<Material> mat_3 = std::make_shared<Metal>(tinyMath::vec3f(0.7f, 0.6f,0.5f), 0.0f);

    std::shared_ptr<Object> obj_ground = std::make_shared<Sphere>(tinyMath::vec3f(0.0f, -1000.0f, 0.0f), 1000.0f, mat_ground);
    std::shared_ptr<Object> obj_1 = std::make_shared<Sphere>(tinyMath::vec3f(0.0f, 1.0f, 0.0f), 1.0f, mat_1);
    std::shared_ptr<Object> obj_2 = std::make_shared<Sphere>(tinyMath::vec3f(-4.0f, 1.0f, 0.0f), 1.0f, mat_2); // make bubble use radisu 0.5 and radius -0.4
    std::shared_ptr<Object> obj_3 = std::make_shared<Sphere>(tinyMath::vec3f(4.0f, 1.0f, 0.0f), 1.0f, mat_3);
    
    Scene scene;
    scene.addObject(obj_ground);
    scene.addObject(obj_1);
    scene.addObject(obj_2);
    scene.addObject(obj_3);
    random_scene(scene);
    
    
    Renderer render(image);
    auto begin = std::chrono::system_clock::now();
    render.render(scene, camera);
    auto end = std::chrono::system_clock::now();
    printTimeTook(begin, end, "Render");
    
    image->write();
    return 0;
}