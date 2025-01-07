package com.almasb.fxglgames.pong;

import com.almasb.fxgl.entity.component.Component;
import com.almasb.fxgl.dsl.FXGL;
import com.almasb.fxgl.physics.PhysicsComponent;
import javafx.geometry.Point2D;
import static com.almasb.fxgl.dsl.FXGL.*;

public class AIComponent extends Component {
    public enum Role{
        Team_1,
        Team_2
    }

    public enum State{
        IDLE,
        SWITCH_HALF,
        KEEP_APART
    }

    private State state = State.IDLE;
    private Role role;
    private PhysicsComponent physics;

    private double lastX = 0;
    private double lastY = 0;
    private double lastUpdateTime = 0;
    double predictedBallX = 0;
    double predictedBallY = 0;

    public AIComponent(Role role) {
        this.role = role;
    }

    @Override
    public void onUpdate(double tpf) {
        // Don't handle AI movement until a player has moved
        if (FXGL.getGameWorld().getEntitiesByComponent(BatComponent.class).get(0).getX() != 320 || FXGL.getGameWorld().getEntitiesByComponent(BatComponent.class).get(0).getY() != 380 || FXGL.getGameWorld().getEntitiesByComponent(BatComponent.class).get(1).getX() != 940 || FXGL.getGameWorld().getEntitiesByComponent(BatComponent.class).get(1).getY() != 380) {
            handleAIMovement();
        }
    }

    private void handleAIMovement() {
        // Get AI positions
        Point2D AiTeam1 = FXGL.getGameWorld().getEntitiesByComponent(AIComponent.class).get(0).getPosition();
        Point2D AiTeam2 = FXGL.getGameWorld().getEntitiesByComponent(AIComponent.class).get(1).getPosition();

        // Get co-ordinates of AI player
        double x = entity.getX();
        double y = entity.getY();

        // Check if AI is too close to screen edge.
        // Move away if too close, handle normal movement otherwise
        if (y < 680 && y > 90 && x > 80 && x < 1200) {
            handleHalfSwitch();
            if (AiTeam1.distance(AiTeam2) > 40 && state!= State.SWITCH_HALF) {
                moveTowardsBall();
            } else if (state != State.SWITCH_HALF) {
                moveAway();
            }
        } else {
            if (x <= 80) {
                physics.setVelocityX(1);
            } else if (x >= 1200) {
                physics.setVelocityX(-1);
            } else if (y <= 90) {
                physics.setVelocityY(1);
            } else if (y >= 680) {
                physics.setVelocityY(-1);
            }
        }
    }

    private void handleHalfSwitch() {
        // Handle for Team1 AI
        if (role == Role.Team_1) {
            if(FXGL.getGameWorld().getEntitiesByComponent(BatComponent.class).get(0).getX() <= 640 && FXGL.getGameWorld().getEntitiesByComponent(AIComponent.class).get(0).getX() <= 640){
                state = State.SWITCH_HALF;
                physics.setVelocityX(80);
            }
            else if (FXGL.getGameWorld().getEntitiesByComponent(BatComponent.class).get(0).getX() > 640 && FXGL.getGameWorld().getEntitiesByComponent(AIComponent.class).get(0).getX() > 640) {
                physics.setVelocityX(-80);
                state = State.SWITCH_HALF;
            } else {
                if (state == State.SWITCH_HALF) {
                    physics.setVelocityX(0);
                }
                state = State.IDLE;
            }
        }

        // Handle for Team2 AI
        if (role == Role.Team_2) {
            if (FXGL.getGameWorld().getEntitiesByComponent(BatComponent.class).get(1).getX() <= 640 && FXGL.getGameWorld().getEntitiesByComponent(AIComponent.class).get(1).getX() <= 640) {
                state = State.SWITCH_HALF;
                physics.setVelocityX(80);
            } else if (FXGL.getGameWorld().getEntitiesByComponent(BatComponent.class).get(1).getX() > 640 && FXGL.getGameWorld().getEntitiesByComponent(AIComponent.class).get(1).getX() > 640) {
                state = State.SWITCH_HALF;
                physics.setVelocityX(-80);
            } else {
                if (state == State.SWITCH_HALF) {
                    physics.setVelocityX(0);
                }
                state = State.IDLE;
            }
        }
    }

    private void moveTowardsBall() {

        double BallX = FXGL.getGameWorld().getEntitiesByComponent(BallComponent.class).get(0).getX();
        double BallY = FXGL.getGameWorld().getEntitiesByComponent(BallComponent.class).get(0).getY();

        double currentTime = FXGL.getGameTimer().getNow();

        if (state != State.SWITCH_HALF) {
            if (lastX != 0 && lastY != 0) {
                double ballVelocityX = (BallX - lastX) * 60;
                double ballVelocityY = (BallY - lastY) * 60;

                // Only recalculate if it's been 1 second since last
                if (currentTime - lastUpdateTime >= 1) {
                    lastUpdateTime = currentTime;
                    predictedBallX = BallX + (ballVelocityX * 2);
                    predictedBallY = BallY + (ballVelocityY * 2);
                }

                if((entity.getX() <= 640 && predictedBallX <= 640) || (entity.getX() > 640 && predictedBallX > 640)) {
                        if (Math.abs(entity.getX() - predictedBallX) > 0) {
                            if (predictedBallX > entity.getX()) {
                                physics.setVelocityX(80);
                            } else if (predictedBallX < entity.getX()) {
                                physics.setVelocityX(-80);
                            } else {
                                physics.setVelocityX(0);
                            }
                        } else {
                            physics.setVelocityX(0);
                        }


                        if (Math.abs(entity.getY() - predictedBallY) > 0) {
                            if (predictedBallY > entity.getY()) {
                                physics.setVelocityY(80);
                            } else if (predictedBallY < entity.getY()) {
                                physics.setVelocityY(-80);
                            } else {
                                physics.setVelocityY(0);
                            }
                        } else {
                            physics.setVelocityY(0);
                        }

                } else {
                    physics.setVelocityX(0);
                    physics.setVelocityY(0);
                }
            }
        }
        lastX = BallX;
        lastY = BallY;

    }

    private void moveAway(){
        // Get Positions
        Point2D AiTeam1Pos = FXGL.getGameWorld().getEntitiesByComponent(AIComponent.class).get(0).getPosition();
        Point2D AiTeam2Pos = FXGL.getGameWorld().getEntitiesByComponent(AIComponent.class).get(1).getPosition();

        if (role == Role.Team_1) {
            if (AiTeam1Pos.getX() < AiTeam2Pos.getX()) {
                physics.setVelocityX(60);
            } else if (AiTeam1Pos.getX() > AiTeam2Pos.getX()) {
                physics.setVelocityX(-60);
            } else {
                physics.setVelocityX(60);
            }
        }

        if (role == Role.Team_1) {
            if (AiTeam1Pos.getY() < AiTeam2Pos.getY()) {
                physics.setVelocityY(60);
            } else if (AiTeam1Pos.getY() > AiTeam2Pos.getY()) {
                physics.setVelocityY(-60);
            } else {
                physics.setVelocityY(60);
            }
        }

        if (role == Role.Team_2) {
            if (AiTeam2Pos.getX() < AiTeam1Pos.getX()) {
                physics.setVelocityX(60);
            } else if (AiTeam2Pos.getX() > AiTeam1Pos.getX()) {
                physics.setVelocityX(-60);
            } else {
                physics.setVelocityX(-60);
            }
        }

        if (role == Role.Team_2) {
            if (AiTeam2Pos.getY() < AiTeam1Pos.getY()) {
                physics.setVelocityY(60);
            } else if (AiTeam2Pos.getY() > AiTeam1Pos.getY()) {
                physics.setVelocityY(-60);
            } else {
                physics.setVelocityY(-60);
            }
        }

        state = State.KEEP_APART;
        physics.setLinearVelocity(physics.getVelocityX() * -1,physics.getVelocityY() * -1);
    }

    public void resetPosition(int player) {
        if (player == 1){
            physics.overwritePosition(new Point2D(
                    getAppWidth() / 2 - 100, 180
            ));
        } else if (player == 2){
            physics.overwritePosition(new Point2D(
                    getAppWidth() / 2 + 100, 580
            ));
        }
    }
}