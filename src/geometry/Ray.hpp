#pragma once

#include "Vector.hpp"

/**
 * define a ray with o + t * d
*/
struct Ray
{
    Ray(const tinyMath::vec3f& origin, const tinyMath::vec3f& direction)
        :o(origin), d(direction){}
    
    const tinyMath::vec3f o;
    const tinyMath::vec3f d;
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
    };
    ~IntersectResult(){}
    float t; // the t of o + t * d
    tinyMath::vec3f coords; // the hit points coords
    tinyMath::vec3f normal; // the hit points normal
    bool isFrontFace; // wheather ray is cast from outward
    
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
