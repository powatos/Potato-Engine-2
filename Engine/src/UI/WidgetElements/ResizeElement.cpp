/** @file "ResizeElement.cpp" */

#include "UI/ResizeElement.hpp"

ResizeElement::ResizeElement(const std::string& UID) : WidgetElement(UID), padding(1) {
    SetScreenPosition(UIVector());
    SetScreenSize(UIVector());
}

void ResizeElement::TickPostPhysics(float dt) {

    UIVector TL, BR; // top left pos, bottom right pos
    Vector2 tTL, tBR; // true top left pos, true bottom right pos

    for (const auto& sibling : ___hierarchyRef->Parent->Children) {
        if (sibling->Object == nullptr || sibling->Object == this || sibling->Object == ResizeTarget) { continue; }

        const UIVector TL_siblingPos = sibling->Object->GetScreenPosition();
        const Vector2 TL_tSiblingPos = sibling->Object->GetTruePosition();
        const UIVector BR_siblingPos = TL_siblingPos + sibling->Object->GetScreenSize();
        const Vector2 BR_tSiblingPos = TL_tSiblingPos + sibling->Object->GetTrueSize();


        if (TL_tSiblingPos.x < tTL.x) {
            tTL.x = TL_tSiblingPos.x;
            TL.scale.x = TL_siblingPos.scale.x;
            TL.offset.x = TL_siblingPos.offset.x;
        }
        if (TL_tSiblingPos.y < tTL.y) {
            tTL.y = TL_tSiblingPos.y;
            TL.scale.y = TL_siblingPos.scale.y;
            TL.offset.y = TL_siblingPos.offset.y;
        }

        if (BR_tSiblingPos.x > tBR.x) {
            tBR.x = BR_tSiblingPos.x;
            BR.scale.x = BR_siblingPos.scale.x;
            BR.offset.x = BR_siblingPos.offset.x;
        }
        if (BR_tSiblingPos.y > tBR.y) {
            tBR.y = BR_tSiblingPos.y;
            BR.scale.y = BR_siblingPos.scale.y;
            BR.offset.y = BR_siblingPos.offset.y;
        }

    }

    ResizeTarget->SetScreenPosition(TL - UIVector(0, 0, padding, padding));
    ResizeTarget->SetScreenSize((BR - TL) + UIVector(0, 0, 2*padding, 2*padding));

}
