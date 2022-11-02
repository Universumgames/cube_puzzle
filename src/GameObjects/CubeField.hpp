//
// Created by Tom Arlt on 02.11.22.
//
#pragma once
#include "GameObject.hpp"

class CubeField final: public GameObject{
public:
    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;
};
