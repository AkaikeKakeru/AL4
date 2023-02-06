#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"

CollisionManager* CollisionManager::GetInstance() {
    static CollisionManager instance;
    return &instance;
}

void CollisionManager::CheckAllCollision() {
    std::forward_list<BaseCollider*>::iterator itA;
    std::forward_list<BaseCollider*>::iterator itB;

    //全ての組み合わせについて総当たりチェック
    itA = colliders_.begin();
    for ( ; itA != colliders_.end(); ++itA) {
        itB = itA;
        ++itB;
        for (; itB != colliders_.end(); ++itB) {
            BaseCollider* colA = *itA;
            BaseCollider* colB = *itB;

            //ともに球
            if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
                colB->GetShapeType() == COLLISIONSHAPE_SPHERE) {
                CheckSphere2Sphere(colA, colB);
            }
        }
    }
}

void CollisionManager::CheckSphere2Sphere(BaseCollider* colA,BaseCollider* colB) {
    Sphere* SphereA = dynamic_cast<Sphere*>(colA);
    Sphere* SphereB = dynamic_cast<Sphere*>(colB);
    Vector3 inter;
    if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter)) {
        colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
        colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
    }
}
