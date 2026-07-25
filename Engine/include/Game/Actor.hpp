/** @file Actor.hpp */
#pragma once

#include "Core/Archivable.hpp"
#include "Core/Tickable.hpp"
#include "Game/HitResult.hpp"
#include "Core/Texture.hpp"

#include "Util/Vector2.hpp"

/**
 * @brief Enum representing movability of an actor
 */
enum class ActorMovability
{
    Movable, ///< @brief Physics can move actor 
    Static ///< @brief Actor is physics-immovable
};

/**
 * @brief Enum representing collision response of an actor
 */
enum class CollisionType
{
    Overlap, ///< @brief Actor generates hit events without collision correction
    Block, ///< @brief Actor generates hit events with collision correction
    None ///< @brief Actor doesn't collide nor generate hit events
};

/**
 * @brief Base actor class
 * @details This class represents a level object that exists in the game world. All objects that obey world
 * physics must inherit from Actor base
 */
class Actor : public Archivable, public Tickable
{
public:
    
    /**
     * @brief Constructs actor object
     * @details Put all class level functionality here (member initializations, 
     * default values, etc.)
     * @warning Never construct actors manually. @sa World::SpawnActor
     */
    Actor();
    virtual ~Actor();
    
    Vector2 GetPosition() const; /**< @brief Gets position @returns Position */
    void SetPosition(const Vector2& position); /**< @brief Sets position @param position Position to set */
    void AddLocalOffset(const Vector2& offset); /**< @brief Adds to location vector @param offset Offset to add */

    Vector2 GetSize() const; /**< @brief Gets size @returns Size */
    void SetSize(const Vector2& size); /**< @brief Sets size @param size Size to set */
    void ResizeToTexture(); /**< @brief Resizes the actor to match the texture bounding box */

    bool isVisible() const; /**< @brief Checks if actor visible @returns true if visible */
    void SetVisibility(bool visibility); /**< @brief Sets visibility of actor @param visibility Visibility to set */

    Vector2 GetVelocity() const; /**< @brief Gets velocity @returns Velocity */
    void SetVelocity(const Vector2& velocity); /**< @brief Sets velocity @param velocity Velocity to set */
    void AddImpulse(const Vector2& force); /**< @brief Adds impulse to velocity @param force Impulse to add */
    
    float GetMass() const; /**< @brief Gets mass @returns Mass */
    void SetMass(float mass); /**< @brief Sets mass @param mass Mass to set */

    bool isSimulatingPhysics() const; /**< @brief Checks if simulating physics @returns true if simulating physics */
    void SetSimulatingPhysics(bool enabled); /**< @brief Sets physics simulation @param enabled Whether to simulate physics */

    Vector2 GetForces() const; /**< @brief Gets forces applied to actor in current tick @returns Forces */
    void AddForce(const Vector2& force); /**< @brief Adds force to actor @param force Force to add */
    void ClearForces(); /**< @brief Clears all forces applied to actor */

    bool IsUsingAsymmetricGravity() const; /**< @brief Checks if actor is using asymmetric gravity @returns true if using asymmetric gravity */
    void SetUseAsymmetricGravity(bool useAsymmetricGravity); /**< @brief Sets whether actor should use asymmetric gravity @param useAsymmetricGravity Whether to use asymmetric gravity */

    ActorMovability GetMovability() const; /**< @brief Gets movability @returns Movability */
    void SetMovability(ActorMovability movability); /**< @brief Sets movability @param movability Movability to set */

    CollisionType GetCollisionType() const; /**< @brief Gets collision type @returns Collision type */
    void SetCollisionType(CollisionType collisionType); /**< @brief Sets collision type @param CollisionType Collision type to set */

    float GetBounce() const; /**< @brief Gets bounce @returns Bounce */
    void SetBounce(float bounce); /**< @brief Sets bounce @param bounce Bounce to set */

    Texture& GetTexture();
    void SetTexture(const std::string& textureName);

    bool IsUsingCTex() const;
    void SetUsingCTex(bool enabled);

    /**
     * @brief Internal function used to queue BeginPlay() on actor
     */
    void DispatchBeginPlay();

    /**
     * @brief Called internally when actor collides with another actor
     * @warning This function is meant for internal calls only. Do not call this function manually.
     * @note Override to implement hit functionality and always call base method:
     * @code
     * void ActorClass::OnHit(const HitResult& hitResult) {
     *    BaseActor::OnHit(hitResult);
     *    // ...
     * }
     * @endcode
     * @param hitResult Information about the hit
     */
    virtual void OnHit(const HitResult& hitResult);

    /**
     * @brief Single-char texture for actor 
     * @details The actor is displayed as a grid (based on its @ref GetSize() "size") of this character
     * @note This is intended to render simple objects or debug actors easily. For detailed textures, TODO
     */
    char ctex;
protected:

    /**
     * @brief Gameplay start
     * @details Put all pre-gameplay functionality here
     * @note Always call base method when overriding:
     * @code
     * void ActorClass::BeginPlay() {
     *     BaseActor::BeginPlay();
     *     // ...
     * }
     * @endcode
     */
    virtual void BeginPlay();
    
    virtual void Tick(float dt) override;

private:

    Vector2 Position;
    Vector2 Velocity;
    Vector2 Forces;
    Vector2 Size;
    
    ActorMovability Movability;
    CollisionType CollisionResponse;

    float Bounciness;
    float Mass;
    bool Visible;
    bool SimulatePhysics;
    bool UseAsymmetricGravity;

    bool isInPlay;

    Texture ActorTexture;
    bool bUseCTex;
   
};