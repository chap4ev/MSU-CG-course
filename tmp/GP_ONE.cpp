#include "GP_ONE.h"

Sprite   GP_ONE::spriteMemory[MAX_SPRITE_COUNT];
uint16_t GP_ONE::frameBuffer[FRAMEBUFFER_BUF_SIZE];


void GP_ONE::loadSprites(const Sprite *sprites, uint16_t spriteCount) {
    // spriteCount overflow check
    if (spriteCount > MAX_SPRITE_COUNT) {
        spriteCount = MAX_SPRITE_COUNT;
    }
    std::copy(sprites, sprites + spriteCount, spriteMemory);
}


void GP_ONE::clearFrameBuffer(BackGroundColor bkgColor) {
    // binary color tile filling
    uint16_t filledTile = ((uint16_t) bkgColor == 0) ? 0 : ~uint16_t(0);

    std::fill_n(frameBuffer, FRAMEBUFFER_BUF_SIZE, filledTile);
}


void GP_ONE::drawSpriteInstances(const SpriteInstance *instances, uint16_t instanceCount) {
    for (uint16_t i = 0; i < instanceCount; ++i) {
        uint16_t *spriteAlpha = spriteMemory[instances[i].ind].alpha;
        uint16_t *spriteColor = spriteMemory[instances[i].ind].color;

        uint16_t shift = instances[i].x & 15u;  // x % 16
        uint16_t rShift = 16u - shift;

        uint16_t tileX = instances[i].x >> 4u;   // x / 16

        uint16_t frameBufferTileIndY = instances[i].y << 5u; //  instances[i].y * FRAMEBUFFER_TILES_X
        uint16_t spriteTileIndY = 0;

        for (uint16_t j = 0; j < SPRITE_TILES_Y; ++j) {
            uint16_t frameBufferTileIndYX = frameBufferTileIndY + tileX;
            uint16_t spriteTileIndYX = spriteTileIndY;
            for (uint16_t k = 0; k < SPRITE_TILES_X; ++k) {
                uint16_t spriteTileAlpha = spriteAlpha[spriteTileIndYX] >> shift;
                uint16_t spriteTileColor = spriteColor[spriteTileIndYX] >> shift;
                frameBuffer[frameBufferTileIndYX] =
                        (frameBuffer[frameBufferTileIndYX] & ~spriteTileAlpha) | (spriteTileColor & spriteTileAlpha);

                ++frameBufferTileIndYX;

                if (shift != 0) {
                    spriteTileAlpha = spriteAlpha[spriteTileIndYX] << rShift;
                    spriteTileColor = spriteColor[spriteTileIndYX] << rShift;
                    frameBuffer[frameBufferTileIndYX] =
                            (frameBuffer[frameBufferTileIndYX] & ~spriteTileAlpha) |
                            (spriteTileColor & spriteTileAlpha);
                }

                ++spriteTileIndYX;
            }
            frameBufferTileIndY += FRAMEBUFFER_TILES_X;
            spriteTileIndY += SPRITE_TILES_X;
        }
    }
}


void GP_ONE::saveFrameBuffer(FrameBuffer &outFrameBuffer) {
    std::copy(frameBuffer, frameBuffer + FRAMEBUFFER_BUF_SIZE, outFrameBuffer.color);
}