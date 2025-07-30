#include "raylib.h"
#include "box2d/box2d.h"

Vector2 ToVector2(const b2Vec2& vector) {
    return {vector.x, vector.y};
}
b2Vec2 ToB2Vec(const Vector2& vector) {
    return {vector.x, vector.y};
}

struct player{
    Rectangle Draw = {350, 0, 100, 100};
};

int main(){
    
    InitWindow(800, 800, "Angel Game");

    b2DebugDraw* debugDraw;

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){0.0f, 10.0f};
    b2WorldId worldId = b2CreateWorld(&worldDef);
    
    b2BodyDef groundBodyDef = b2DefaultBodyDef();
    groundBodyDef.position = (b2Vec2){0.0f, 810.0f};
    b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);
    b2Polygon groundBox = b2MakeBox(800.0f, 10.0f);

    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = (b2Vec2){350.0f, 0.0f};
    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

    b2Polygon dynamicBox = b2MakeBox(100.0f, 100.0f);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 20.0f;
    shapeDef.material.friction = 0.3f;

    b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);

    int subStepCount = 80;
    //It is retarded to tie physics time to dT
    float timeStep = 1.0f / 60.0f;

    player Player;

    SetTargetFPS(60);

    while (!WindowShouldClose()){

        b2World_Step(worldId, timeStep, subStepCount);

        Player.Draw.x = b2Body_GetPosition(bodyId).x;
        Player.Draw.y = b2Body_GetPosition(bodyId).y;

        BeginDrawing();
            DrawRectangleRec(Player.Draw, WHITE);
        EndDrawing();

        ClearBackground(BLACK);
    }

    b2DestroyWorld(worldId);

    CloseWindow();

    return 0;
}