#include <emmintrin.h>
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

        uint16_t shift16 = instances[i].x & 15u;  // x % 16
        uint16_t shift16R = 16u - shift16;

        uint16_t spriteTileInd = 0;

        // instances[i].y * FRAMEBUFFER_TILES_X + x / 16
        uint16_t frameBufferTile16Ind = (instances[i].y << 5u) + (instances[i].x >> 4u);

        for (uint16_t j = 0; j < SPRITE_TILES_Y; ++j) {
            __m128i fb128tile = _mm_loadu_si128((__m128i const *) (frameBuffer + frameBufferTile16Ind));

            // TODO  _mm_loadl_epi64 use on aligned frameBuffer
            __m128i spriteTileAlpha128 = _mm_loadu_si64((void const *) (spriteAlpha + spriteTileInd));
            __m128i spriteTileColor128 = _mm_loadu_si64((void const *) (spriteColor + spriteTileInd));

            if (shift16 != 0) {
                // shift alpha channel on shift16 bits
                spriteTileAlpha128 = _mm_or_si128(_mm_srli_epi16(spriteTileAlpha128, shift16),
                                                  _mm_slli_epi16(_mm_bslli_si128(spriteTileAlpha128, 2), shift16R));

                // shift color on shift16 bits
                spriteTileColor128 = _mm_or_si128(_mm_srli_epi16(spriteTileColor128, shift16),
                                                  _mm_slli_epi16(_mm_bslli_si128(spriteTileColor128, 2), shift16R));
            }

            // sprite tile render
            fb128tile = _mm_or_si128(_mm_andnot_si128(spriteTileAlpha128, fb128tile),
                                     _mm_and_si128(spriteTileColor128, spriteTileAlpha128));

            // load in frameBuffer
            _mm_storeu_si128((__m128i *) (frameBuffer + frameBufferTile16Ind), fb128tile);

            frameBufferTile16Ind += FRAMEBUFFER_TILES_X;
            spriteTileInd += SPRITE_TILES_X;
        }
    }
}


void GP_ONE::saveFrameBuffer(FrameBuffer &outFrameBuffer) {
    std::copy(frameBuffer, frameBuffer + FRAMEBUFFER_BUF_SIZE, outFrameBuffer.color);
}