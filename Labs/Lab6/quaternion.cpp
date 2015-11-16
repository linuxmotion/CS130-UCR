#include "quaternion.h"

#include <cassert>
#include <cmath>
#include <iostream>

using namespace std;

quaternion::quaternion()
    : s(1), x(0), y(0), z(0)
{
}

quaternion::~quaternion()
{
}

quaternion::quaternion(
    float angle, float x, float y, float z
    )
{
    from_angle_axis(angle, x, y, z);
}
 
void quaternion::slerp(
    float u, const quaternion& p, const quaternion& q
    )
{
    
    quaternion v0 = p;
    quaternion v1 = q;
    // Find length of vectors
   // float Plength = sqrt(powf(p.x, 2) + powf(p.y,2) + powf(p.z,2));
   // float Qlength =  sqrt(powf(p.x, 2) + powf(p.y,2) + powf(p.z,2));
    // compute the dot product between them
    float dotp = v0.dot(v1);
    cout << "Dot product is " << dotp << " \n";
    if(dotp > 0.9995){
        
        cout << "we have very close \n";
    }
   
    // first clamp dot to the value [-1,1] for arcos
    if(dotp < -1)
        dotp = -1;
    else if(dotp > 1)
        dotp = 1;
     // find the angle between p and q
    
    float theta_0 = acos(dotp);
    float theta = theta_0*u; 
    cout << "theta_0 = " << theta_0 << " theta = " << theta << "\n";
    v0.scale(dotp);
    //cout << v0 << "\n";
    quaternion v2 = v1;
    v2.sub(p); // end - start*dot
    //cout << v2 << endl;
    v2.normalize();
    //cout << v2 << endl;
    v2.scale(sin(theta));
    v0.scale(cos(theta));
    v0.add(v2);
   // cout << v0 << endl;
    // this is the final quaternion
    quaternion fin = v0;
    cout << fin << endl;
    *this = fin;
   // cout << "checking slerp " << *this << endl;
    
}
    
void quaternion::from_angle_axis(
    float angle, float x, float y, float z
    )
{
    float xnorm, ynorm, znorm;
    float mag = sqrt(x*x + y*y + z*z);
    assert(mag > 0);
    xnorm = x/mag; ynorm = y/mag; znorm = z/mag;

    float angle2 = angle/2 * (M_PI/180.0);
    float sina2 = sin(angle2);

    s = cos(angle2);
    this->x = xnorm*sina2;
    this->y = ynorm*sina2;
    this->z = znorm*sina2;
}
    
void quaternion::to_angle_axis(
    float &angle, float &x, float &y, float &z
    ) const
{

    float angle2 = acos(s); // half angle
    float sina2 = sin(angle2);

    angle = 2*angle2 * (180/M_PI);

    if (angle == 0) {
        // singularily, arbitrarily pick the x-axis
        x = 1; y = 0; z = 0;
    } else {
        x = this->x / sina2;
        y = this->y / sina2;
        z = this->z / sina2;
    }
}
    
void quaternion::scale(float a)
{
    s *= a;
    x *= a;
    y *= a;
    z *= a;
}

void quaternion::add(const quaternion& q)
{
    s += q.s;
    x += q.x;
    y += q.y;
    z += q.z;
}

void quaternion::add(const quaternion& p, const quaternion& q)
{
    set(p);
    add(q);
}

void quaternion::sub(const quaternion& q)
{
    s -= q.s;
    x -= q.x;
    y -= q.y;
    z -= q.z;
}

void quaternion::sub(const quaternion& p, const quaternion& q)
{
    set(p);
    sub(q);
}
    
void quaternion::mult(const quaternion& p, const quaternion& q)
{
    s = p.s * q.s - (p.x * q.x + p.y * q.y + p.z * q.z);
    x = p.s * q.x + p.x * q.s + (p.y * q.z - p.z * q.y);
    y = p.s * q.y + p.y * q.s + (p.z * q.x - p.x * q.z);
    z = p.s * q.z + p.z * q.s + (p.x * q.y - p.y * q.x);
}
    
void quaternion::mult(const quaternion& q)
{
    quaternion p(*this);
    mult(p, q);
}
    
float quaternion::dot(const quaternion& q) const
{
    return ::dot(*this, q);
}
    
void quaternion::normalize()
{
    float mag = sqrt(::dot(*this, *this));
    assert(mag != 0);

    s /= mag; x /= mag; y /= mag; z /= mag;
}
    
void quaternion::inv()
{
    x = -x;
    y = -y;
    z = -z;
}
    
void quaternion::pow(float r)
{
    float angle,x,y,z;
    to_angle_axis(angle,x,y,z);
    angle *= r;
    from_angle_axis(angle,x,y,z);
}
    
void quaternion::set(const quaternion& p)
{
    s = p.s; x = p.x; y = p.y; z = p.z;
}
    
void quaternion::set(float s, float x, float y, float z)
{
    this->s = s; this->x = x; this->y = y; this->z = z;
}

void quaternion::get(float &s, float &x, float &y, float &z) const
{
    s = this->s; x = this->x; y = this->y; z = this->z;
}

float dot(const quaternion& p, const quaternion& q)
{
    return p.s*q.s + p.x*q.x + p.y*q.y + p.z*q.z;
}

std::ostream& operator<<(std::ostream& ostr, const quaternion& q)
{
    float s, x, y, z;
    q.get(s, x, y, z);
    ostr << s << " " << x << " " << y << " " << z;
    return ostr;
}

