#pragma once

static float lerp(float start, float end, float delta) {
	return start * (1 - delta) + end * delta;
}

static float Rad2Deg = 180.0 / 3.141592653589793238463;
static float Deg2Rad = 3.141592653589793238463 / 180.0;
