#include"Node.h"
#include"Planet.h"
#include"Urban.h"
#include"BiomeData.h"

Array<BiomeData>	biomeData;
Array<Node> nodes;

void Main()
{
	{
		Window::SetTitle(L"Planet Viewer");

		INIReader iniReader(L"config.ini");
		if (iniReader.getOr<bool>(L"Window.FullScreen", false)) Window::SetFullscreen(true, Graphics::GetFullScreenSize().back());
		else Window::Resize(iniReader.getOr<Size>(L"Window.WindowSize", Size(1280, 720)));
	}

	Texture mapTexture;

	//Nodeの読み込み
	{
		BinaryReader reader(L"assets/nodemap/nodeMap.bin");
		int	nodesSize;

		reader.read(nodesSize);
		for (int i = 0; i < nodesSize; ++i)
		{
			Vec3 ePos;
			reader.read(ePos);
			nodes.emplace_back(ePos);
		}
	}

	//BiomeData
	{
		JSONReader json(L"assets/data/biomeData.json");
		for (auto j : json[L"BiomeData"].getArray())
			biomeData.emplace_back(j);
	}
	//Mapデータの選択
	for (auto item : FileSystem::DirectoryContents(L"map/"))
	{
		if (FileSystem::IsDirectory(item) && FileSystem::Exists(item + L"BiomeData.bin"))
		{
			//バイオームデータのロード
			{
				BinaryReader reader(item + L"BiomeData.bin");

				for (auto& n : nodes)
				{
					reader.read(n.biomeType);

					//Nodeに色の適用
					n.color = n.data().color.lerp(RandomColor(), 0.05);
				}
			}

			break;
		}
	}

	//VoronoiMapの読み込み
	{
		Image	image(L"assets/nodemap/voronoiMap.png");

		for (auto p : step(image.size))
		{
			auto& n = nodes.at(image[p.y][p.x].r + (image[p.y][p.x].g << 8) + (image[p.y][p.x].b << 16));

			image[p.y][p.x] = n.color;
		}

		mapTexture = Texture(image);
	}

	Array<Particle> particles;

	{
		particles.emplace_back(Vec3(0, 0, 0), 5.0, Palette::Orange);

		double minDistance = 100000.0;
		for (int i = 0; i < 3000; i++)
		{
			Color color = Palette::White;

			for (;;)
			{
				auto p = Vec3(Random(-1.0, 1.0), Random(-1.0, 1.0), Random(-1.0, 1.0));

				if (p.length() < 1.0)
				{
					particles.emplace_back(p.normalized()*minDistance*Random(0.1, 0.5), 100, color);
					break;
				}
			}
		}
	}
	const Texture texture(L"assets/image/particle.png", TextureDesc::For3D);

	Graphics3D::SetBlendStateForward(BlendState::Additive);
	Graphics3D::SetDepthStateForward(DepthState::TestOnly);



	Sphere	s(1.0);
	Camera	camera;
	double	timer = 0.0;
	double	timeSpeed = Pow(0.5, 10);
	double	angleX = 0;
	double	angleY = 0;
	double	distance = 4;
	Vec3	sPos = Vec3::Right*1.2;
	camera.lookat = Vec3::Zero;
	camera.pos = Vec3(4, 0, 0);


	while (System::Update())
	{
		ClearPrint();
		Println(L"KeyASDW KeyF1, keyF2");
		Println(int(timer / 360), L"年 ", int(int(timer / 30) % 12 + 1), L"月", int(int(timer) % 30 + 1), L"日", int(int(timer * 24) % 24), L"時");

		Vec2 planetPos = Vec2(100.0, 0.0).rotated(timer / 360.0*TwoPi);

		if (Input::KeyD.pressed) angleX += 0.025;
		if (Input::KeyA.pressed) angleX -= 0.025;
		if (Input::KeyW.pressed) angleY = Min(angleY + 0.025, HalfPi - 0.01);
		if (Input::KeyS.pressed) angleY = Max(angleY - 0.025, -HalfPi + 0.01);
		if (Input::KeyF1.clicked) timeSpeed = Max(Pow(0.5, 15), timeSpeed*0.5);
		if (Input::KeyF2.clicked) timeSpeed = Min(1.0, timeSpeed*2.0);
		distance *= (1 + double(Mouse::Wheel())*0.05);
		distance = Max(1.2, distance);
		timer += timeSpeed;

		sPos = sPos.lerp(Vec3(Cos(angleX)*Cos(angleY), Sin(angleY), Sin(angleX)*Cos(angleY))*distance, 0.1);

		camera.pos = Vec3(planetPos.x, 0, planetPos.y) + sPos;
		camera.lookat = Vec3(planetPos.x, 0, planetPos.y);

		Graphics3D::SetCamera(camera);
		Graphics3D::SetAmbientLight(ColorF(0.10));
		Graphics3D::SetLight(0, Light::Point(Vec3::Zero, 500.0, ColorF(0.75)));

		s.asMesh().rotated(Quaternion::Yaw(-timer*TwoPi).roll(-23.4_deg)).translated(Vec3(planetPos.x, 0, planetPos.y)).draw(mapTexture);

		Graphics3D::DrawParticlesForward(particles, texture);
	}
}

Node::Node(const Pos& _pos)
	: pos(_pos)
	, biomeType(0)
{}

BiomeData&	Node::data() const
{
	return biomeData[biomeType];
}