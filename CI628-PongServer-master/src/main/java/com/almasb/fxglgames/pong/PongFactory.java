/*
 * The MIT License (MIT)
 *
 * FXGL - JavaFX Game Library
 *
 * Copyright (c) 2015-2017 AlmasB (almaslvl@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

package com.almasb.fxglgames.pong;

import com.almasb.fxgl.entity.Entity;
import com.almasb.fxgl.entity.EntityFactory;
import com.almasb.fxgl.entity.SpawnData;
import com.almasb.fxgl.entity.Spawns;
import com.almasb.fxgl.entity.components.CollidableComponent;
import com.almasb.fxgl.physics.BoundingShape;
import com.almasb.fxgl.physics.HitBox;
import com.almasb.fxgl.physics.PhysicsComponent;
import com.almasb.fxgl.physics.box2d.dynamics.BodyType;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Rectangle;

import static com.almasb.fxgl.dsl.FXGL.*;


public class PongFactory implements EntityFactory {

    @Spawns("ball")
    public Entity newBall(SpawnData data) {
        PhysicsComponent physics = new PhysicsComponent();
        physics.setBodyType(BodyType.DYNAMIC);

        return entityBuilder(data)
                .type(EntityType.BALL)
                .bbox(new HitBox(BoundingShape.circle(5)))
                .view(new Circle(5, 5, 5, Color.BLUE))
                .with(physics)
                .with(new CollidableComponent(true))
                .with(new BallComponent())
                .build();
    }

    @Spawns("bat")
    public Entity newBat(SpawnData data) {
        boolean isPlayer = data.get("isPlayer");

        PhysicsComponent physics = new PhysicsComponent();
        physics.setBodyType(BodyType.KINEMATIC);

        return entityBuilder(data)
                .type(isPlayer ? EntityType.PLAYER_BAT : EntityType.ENEMY_BAT)
                .viewWithBBox(new Rectangle(10, 36, Color.LIGHTGRAY))
                .with(new CollidableComponent(true))
                .with(physics)
                .with(new BatComponent())
                .build();
    }

    @Spawns("goalLine")
    public Entity newGoalLine(SpawnData data) {
        PhysicsComponent physics = new PhysicsComponent();
        physics.setBodyType(BodyType.STATIC);

        return entityBuilder(data)
                .type(EntityType.GOAL_LINE)
                .bbox(new HitBox(BoundingShape.box(5, 80)))
                .viewWithBBox(new Rectangle(5, 80, Color.WHITE))
                .with(physics)
                .with(new CollidableComponent(true))
                .build();
    }

    @Spawns("goalNet")
    public Entity newGoalNet(SpawnData data) {
        PhysicsComponent physics = new PhysicsComponent();
        physics.setBodyType(BodyType.STATIC);

        return entityBuilder(data)
                .type(EntityType.GOAL_NET)
                .bbox(new HitBox(BoundingShape.box(40, 80)))
                .viewWithBBox(new Rectangle(40, 80, Color.GREY))
                .with(physics)
                .with(new CollidableComponent(true))
                .build();
    }

    @Spawns("topUiContainer")
    public Entity newTopUiContainer(SpawnData data){
        PhysicsComponent physics = new PhysicsComponent();
        physics.setBodyType(BodyType.STATIC);

        return entityBuilder(data)
                .type(EntityType.SCREEN_TOP_UI_CONTAINER)
                .bbox(new HitBox(BoundingShape.box(getAppWidth(), 2)))
                .viewWithBBox(new Rectangle(getAppWidth(), 2, Color.WHITE))
                .build();
    }

    @Spawns("aiPlayer")
    public Entity newAIPlayer(SpawnData data) {
        boolean isTeamA = data.get("isTeamA");

        PhysicsComponent physics = new PhysicsComponent();
        physics.setBodyType(BodyType.KINEMATIC);

        return entityBuilder(data)
                .type(isTeamA ? EntityType.AI_PLAYER_1 : EntityType.AI_PLAYER_2)
                .viewWithBBox(new Rectangle(10, 36, isTeamA ? Color.YELLOW : Color.GREEN))
                .with(new CollidableComponent(true))
                .with(physics)
                .with(new AIComponent(isTeamA ? AIComponent.Role.Team_1 : AIComponent.Role.Team_2))
                .build();
    }
}
