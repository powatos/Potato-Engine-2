/** @file PlayerController.hpp */
#pragma once

#include "Core/Tickable.hpp"
#include "Game/Camera.hpp"
#include "Game/Player.hpp"

/**
 * @brief Main managing class for the player, controlling interactions and non-local behavior
 * @details 
 */
class PlayerController : public Tickable
{
public:
    /**
     * @internal
     * @brief Constructs player controller
     */
    PlayerController();
    virtual ~PlayerController();

    /** 
     * @brief Initializes controller
     * @details This method is called automatically and is intended for internal use only
     */
    void Initialize();
    /**
     * @brief Assigns player to controller
     * @details This method is called automatically and is intended for internal use only
     * @param player Player to assign
     */
    void AssignPlayer(Player* player);

    /** @brief Gets assigned Player @returns Player */
    Player* GetPlayer() const;
    /** @brief Gets assigned Camera @returns Camera */
    Camera* GetCamera() const;

    /**
     * @brief Called automatically to bind player inputs to InputController
     * @details Override to setup bindings:
     * @code
     * IInputController* controller = PotatoEngine::Get().GetInputController();
     * controller->RegisterInputBinding(
     *     InputBinding(
     *         Keycode::Space, 
     *         InputType::Started,
     *         "JumpAction", 
     *         this, 
     *         &PlayerController::DoJump)
     * );
     * @endcode
     * @note Call base method when overriding to bind default player movement:
     * @code
     * void PlayerControllerClass::SetupInputBindings() {
     *     BasePlayerController::SetupInputBindings();
     *     // ...
     * }
     * @endcode
     */
    virtual void SetupInputBindings();


protected:
    virtual void Tick([[maybe_unused]] float dt) override;

    float TargetMovementVelocity;
    float JumpForce;

    bool CameraFollowsPlayer;
    Vector2 CameraOffset;

private:
    Player* ActivePlayer;
    Camera* ActiveCamera;
    Vector2 playerMoveVec;

    void sMvL(); void eMvL();
    void sMvR(); void eMvR();
    void jump();
    void _rot();

};