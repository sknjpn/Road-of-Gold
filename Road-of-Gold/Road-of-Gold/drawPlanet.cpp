#include"Planet.h"
#include"UI.h"

void	drawPlanet()
{
	for (int i = 0; i < 2; ++i) {
		const auto transformer = tinyCamera.createTransformer(i);

		planet.mapTexture.resize(360_deg, 180_deg).drawAt(0, 0);
		Graphics2D::SetSamplerState(SamplerState::ClampLinear);
	}
}