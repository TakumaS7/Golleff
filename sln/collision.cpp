/*****************************************************************/
//
// 
// collision.cpp（コリジョン処理）
//
//
//
//                    ２０２４年09月26日　製作者名：佐藤琢磨
//
/*****************************************************************/

#include "collision.h"


AABB::AABB(const XMFLOAT3& min, const XMFLOAT3& max)
    : m_min(min), m_max(max)
{
    m_center.x = (max.x + min.x) * 0.5f;
    m_center.y = (max.y + min.y) * 0.5f;
    m_center.z = (max.z + min.z) * 0.5f;
}

AABB AABB::Translation(const XMVECTOR& move) const
{
    AABB aabb;
    XMStoreFloat3(&aabb.m_min, XMLoadFloat3(&m_min) + move);
    XMStoreFloat3(&aabb.m_max, XMLoadFloat3(&m_max) + move);
    XMStoreFloat3(&aabb.m_center, XMLoadFloat3(&m_center) + move);

    return aabb;
}

bool AABB::isOverlap(const AABB& aabb) const
{
    return  m_min.x < aabb.m_max.x &&
            m_max.x > aabb.m_min.x &&
            m_min.y < aabb.m_max.y &&
            m_max.y > aabb.m_min.y &&
            m_min.z < aabb.m_max.z &&
            m_max.z > aabb.m_min.z;
}

bool AABB::isOverlap(const Point& point) const
{
    return  m_min.x < point.GetPosition().x &&
            m_max.x > point.GetPosition().x &&
            m_min.y < point.GetPosition().y &&
            m_max.y > point.GetPosition().y &&
            m_min.z < point.GetPosition().z &&
            m_max.z > point.GetPosition().z;
}

Sphere Sphere::Translation(const XMVECTOR& move) const
{
    XMFLOAT3 center;
    XMStoreFloat3(&center, XMLoadFloat3(&m_Center) + move);

    return { center, m_Radius };
}

bool Sphere::isOverlap(const Point& point) const
{
    XMVECTOR v = XMLoadFloat3(&point.GetPosition()) - XMLoadFloat3(&m_Center);
    
    //XMVector

    return false; // XMVector3LengthSq(v) < m_Radius* m_Radius; //XMVector3LengthSq：斜辺の２乗を返す
}
