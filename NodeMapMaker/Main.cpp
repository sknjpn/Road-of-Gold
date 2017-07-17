#include <Siv3D.hpp> // OpenSiv3D v0.1.5
#include"Node.h"
#include"Pi.h"
#include"Pos.h"
void Main()
{
	const double k = 0.0000001;
	const int s = 30;
	const Font font(16);
	Array<Node*> block[s][s][s];


	Window::Resize(1280, 720);
	for (int i = 0; i < 20000; i++)
	{
		for (;;)
		{
			auto ePos = Vec3(Random(-1.0, 1.0), Random(-1.0, 1.0), Random(-1.0, 1.0));
			if (ePos.length() <= 1.0 && !ePos.isZero())
			{
				nodes.emplace_back(ePos);
				break;
			}
		}
	}
	Stopwatch t(true);

	while (!KeyEnter.pressed() && System::Update())
	{
		font(L"処理を開始するにはEnterキーを押してください。").draw();
	}

	for (int i = 0; i < 50; i++)
	{

		for (int x = 0; x < s; x++)
			for (int y = 0; y < s; y++)
				for (int z = 0; z < s; z++)
					block[x][y][z].clear();

		for (auto& n1 : nodes)
			block[Min(s - 1, int((0.5 + 0.5*n1.pos.ePos.x)*s))][Min(s - 1, int((0.5 + 0.5*n1.pos.ePos.y)*s))][Min(s - 1, int((0.5 + 0.5*n1.pos.ePos.z)*s))].emplace_back(&n1);


		for (int x = 0; x < s; x++)
		{
			for (int y = 0; y < s; y++)
			{
				for (int z = 0; z < s; z++)
				{
					for (int ix = x - 1; ix <= x + 1; ix++)
					{
						if (ix == -1 || ix == s) continue;
						for (int iy = y - 1; iy <= y + 1; iy++)
						{
							if (iy == -1 || iy == s) continue;
							for (int iz = z - 1; iz <= z + 1; iz++)
							{
								if (iz == -1 || iz == s) continue;
								for (auto& n1 : block[x][y][z])
								{
									for (auto& n2 : block[ix][iy][iz])
									{
										const auto p = n1->pos.ePos - n2->pos.ePos;
										if (&n1 != &n2)
										{
											const auto l = p.lengthSq();
											auto f = (n1->pos.ePos - n2->pos.ePos).normalized() / l * k;
											n1->pos = Pos(n1->pos.ePos + f);
											n2->pos = Pos(n2->pos.ePos - f);
										}
									}
								}
								if (t.ms() > 16)
								{
									t.restart();
									font(i, L"回").draw();
									const Transformer2D t1(Mat3x2::Scale(200).translate(Window::Center()));
									for (auto& n1 : nodes)
									{
										Circle(n1.pos.mPos, 0.005).draw();
									}
									if (!System::Update()) return;
								}
							}
						}
					}
				}
			}
		}
	}
	int _sizeX = 8192;
	Grid<int> voronoiMap;
	Image image(_sizeX, _sizeX / 2);
	voronoiMap.resize(_sizeX, _sizeX / 2);
	voronoiMap.fill(-1);

	DynamicTexture tex;
	Stopwatch tmr(true);
	int	LastUpdateA = 0;
	int	LastUpdateB = 0;

	for (auto& n : nodes)
	{
		const auto& p = (n.pos.mPos / TwoPi).movedBy(0.5, 0.25)*_sizeX;
		voronoiMap[int(p.y)][int(p.x)] = n.id;
		image[int(p.y)][int(p.x)] = Color(n.id % 256, (n.id >> 8) % 256, (n.id >> 16) % 256);
	}
	for (;;)
	{
		LastUpdateB = LastUpdateA;
		LastUpdateA = _sizeX / 2;
		bool flag = true;
		for (int i = 0; i < 2; i++)
		{
			auto sp = i == 0 ? step(Size(_sizeX, _sizeX / 4 - LastUpdateB)) : step(Point(0, _sizeX / 4 + LastUpdateB), Size(_sizeX, _sizeX / 4 - LastUpdateB));
			for (auto p1 : sp)
			{
				for (int m = 0; m < 4; m++)
				{
					Point p2 = p1;
					switch (m)
					{
					case 0: p2 = { p1.x - 1,p1.y }; break;
					case 1: p2 = { p1.x + 1,p1.y }; break;
					case 2: p2 = { p1.x ,p1.y - 1 }; break;
					case 3: p2 = { p1.x ,p1.y + 1 }; break;
					}
					//例外判定
					if (p2.y < 0 || p2.y >= _sizeX / 2) continue;
					if (p2.x < 0) p2.x = _sizeX - 1;
					if (p2.x >= _sizeX) p2.x = 0;

					//干渉
					if (voronoiMap[p1.y][p1.x] != voronoiMap[p2.y][p2.x] && voronoiMap[p2.y][p2.x] != -1)
					{
						bool f = false;
						if (voronoiMap[p1.y][p1.x] == -1) f = true;
						else
						{
							const auto& mPos = (p1 / double(_sizeX / 2)).movedBy(-1, -0.5)*Pi;
							const auto& ePos = Vec3(cos(mPos.x)*cos(mPos.y), -sin(mPos.y), sin(mPos.x)*cos(mPos.y));
							if ((nodes[voronoiMap[p1.y][p1.x]].pos.ePos - ePos).length() > (nodes[voronoiMap[p2.y][p2.x]].pos.ePos - (ePos)).length()) f = true;
						}
						if (f)
						{
							flag = false;
							auto& n = nodes[voronoiMap[p2.y][p2.x]];
							image[int(p1.y)][int(p1.x)] = Color(n.id % 256, (n.id >> 8) % 256, (n.id >> 16) % 256);
							voronoiMap[p1.y][p1.x] = voronoiMap[p2.y][p2.x];
							if (LastUpdateA > abs(_sizeX / 4 - p2.y)) LastUpdateA = abs(_sizeX / 4 - p2.y);
						}
					}
				}
			}
		}
		if (tmr.ms() > 16)
		{

			tex.fill(image);
			tex.resize(Window::Size()).draw();
			Window::SetTitle(LastUpdateA);
			if (!System::Update()) return;
			tmr.restart();
		}
		if (flag) break;
	}

	//NodeMapの生成
	{
		for (auto p1 : step(Size(_sizeX, _sizeX / 2)))
		{
			for (int m = 0; m < 4; m++)
			{
				Point p2 = p1;
				switch (m)
				{
				case 0: p2 = { p1.x - 1,p1.y }; break;
				case 1: p2 = { p1.x + 1,p1.y }; break;
				case 2: p2 = { p1.x ,p1.y - 1 }; break;
				case 3: p2 = { p1.x ,p1.y + 1 }; break;
				}
				//例外判定
				if (p2.y < 0 || p2.y >= _sizeX / 2) continue;
				if (p2.x < 0) p2.x = _sizeX - 1;
				if (p2.x >= _sizeX) p2.x = 0;

				if (voronoiMap[p1.y][p1.x] != voronoiMap[p2.y][p2.x] && voronoiMap[p2.y][p2.x] != -1 && voronoiMap[p1.y][p1.x] != -1)
				{
					if (!nodes[voronoiMap[p1.y][p1.x]].paths.any([&voronoiMap, &p2](const Path& p) {return p.childNodeID == voronoiMap[p2.y][p2.x]; }))
					{
						nodes[voronoiMap[p1.y][p1.x]].paths.emplace_back(nodes[voronoiMap[p1.y][p1.x]].id, nodes[voronoiMap[p2.y][p2.x]].id);
						nodes[voronoiMap[p2.y][p2.x]].paths.emplace_back(nodes[voronoiMap[p2.y][p2.x]].id, nodes[voronoiMap[p1.y][p1.x]].id);
					}
				}
			}
		}
		for (auto& n : nodes)
		{
			for (auto& p : n.paths)
				paths.emplace_back(&p);
		}
		Window::SetTitle(paths.size());
		BinaryWriter writer(L"Assets/NodeMap.bin");
		writer.write(int(nodes.size()));
		for (int i = 0; i < int(nodes.size()); i++)
		{
			writer.write(nodes[i].pos.ePos);
		}
		writer.write(int(paths.size()));
		for (int i = 0; i < int(paths.size()); i++)
		{
			writer.write(paths[i]->parentNodeID);
			writer.write(paths[i]->childNodeID);
		}
		writer.close();
	}

	//ファイル書き出し
	image.save(L"Assets/VoronoiMap.png");

	while (System::Update());
}