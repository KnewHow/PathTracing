#pragma once

#include "Vector.hpp"
#include "common/Tools.hpp"

class Material;


/**
 * define a ray with o + t * d
*/
struct Ray
{
    Ray(const tinyMath::vec3f& origin, const tinyMath::vec3f& direction)
        :o(origin), d(direction){}
    Ray()
        :o(tinyMath::vec3f(0.0f)), d(tinyMath::vec3f(0.0f)){}
    
    tinyMath::vec3f o;
    tinyMath::vec3f d;


    static inline Ray sampleWithHemisphere(const tinyMath::vec3f& hitPoint, const tinyMath::vec3f& hitNormal) {
        tinyMath::vec3f random_dir = get_random_vector_in_unit_sphere();
        if(tinyMath::dotProduct(random_dir, hitNormal) < 0.0f) { // sample in hemisphere
            random_dir = -random_dir;
        }
        tinyMath::vec3f dir = (hitPoint + random_dir - hitPoint).normalize();
        Ray r(hitPoint, dir);
        return r;
    }

    Ray& operator=(const Ray& ray) {
        o = ray.o;
        d = ray.d;
        return *this;
    }
};

/**
 * ray intersect with object
*/
struct IntersectResult {
    
    IntersectResult() {
        t = 0.0f;
        coords = tinyMath::vec3f(0.0);
        normal = tinyMath::vec3f(0.0);
        isFrontFace = false;
        material = nullptr;
    };

    ~IntersectResult(){}
    float t; // the t of o + t * d
    tinyMath::vec3f coords; // the hit points coords
    tinyMath::vec3f normal; // the hit points normal
    bool isFrontFace; // wheather ray is cast from outward
    std::shared_ptr<Material> material; // the material of hit point
    
    /**
     * judge front face or back face, as well as, determine the normal direction
     * @param ray the ray casting
     * @param outwardNormal the normal direction is outward
    */
    inline void setFrontFace(const Ray& r, const tinyMath::vec3f& outwardNormal) {
        if(tinyMath::dotProduct(r.d, outwardNormal) > 0.0f) {
            isFrontFace = false;
            normal = -outwardNormal;
        } else {
            isFrontFace = true;
            normal = outwardNormal;
        }
    }
};
