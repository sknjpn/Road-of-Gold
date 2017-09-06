#include"Node.h"
#include"Pi.h"
#include"Pos.h"
#include"HugeImage.h"

void Main()
{
	const double k = 0.0000001;
	const int s = 30;
	const Font font(16);
	Array<Node*> block[s][s][s];


	Window::Resize(1280, 720);
	for (int i = 0; i < 30000; ++i)
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

	for (int i = 0; i < 200; ++i)
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
											if (pow(1 / double(s), 2) < l) continue;
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
	int _sizeX = 8192 * 4;
	HugeImage image(Size(_sizeX, _sizeX / 2));
	Grid<int> voronoiMap(_sizeX, _sizeX / 2, -1);

	DynamicTexture tex;
	Stopwatch timer(true);
	int count = 0;
	for (auto& n : nodes)
	{
		const auto& p = (n.pos.mPos / TwoPi).movedBy(0.5, 0.25)*_sizeX;
		voronoiMap[int(p.y)][int(p.x)] = n.id;
		++count;
		image.at(int(p.y), int(p.x)) = Color(n.id % 256, (n.id >> 8) % 256, (n.id >> 16) % 256);
	}
	for (;;)
	{
		bool flag = true;
		for (int i = 0; i < 4; i++)
		{
			auto imgSize = image.size();
			for (auto pos : step(imgSize))
			{
				Point p = pos;
				switch (i)
				{
				case 1:
					p.x = imgSize.x - 1 - pos.x;
					p.y = pos.y;
					break;
				case 2:
					p.x = pos.x;
					p.y = imgSize.y - 1 - pos.y;
					break;
				case 3:
					p.x = imgSize.x - 1 - pos.x;
					p.y = imgSize.y - 1 - pos.y;
					break;
				}
				auto& v1 = voronoiMap.at(p);
				auto& pixel = image.at(p);
				if (v1 != -1)
				{
					for (auto np : step(p.movedBy(-1, -1), Size(3, 3)))
					{
						if (np == p || np.y < 0 || np.y == _sizeX / 2) continue;
						if (np.x < 0) np.x = _sizeX - 1;
						if (np.x == _sizeX) np.x = 0;

						auto& v2 = voronoiMap.at(np);
						if (v2 == -1)
						{
							image.at(np) = pixel;
							v2 = v1;
							++count;
							flag = false;
						}
						else if (v1 != v2)
						{
							const auto& mPos = (np / double(_sizeX / 2)).movedBy(-1, -0.5)*Pi;
							const auto& ePos = Vec3(cos(mPos.x)*cos(mPos.y), -sin(mPos.y), sin(mPos.x)*cos(mPos.y));
							if ((nodes[v1].pos.ePos - ePos).length() < (nodes[v2].pos.ePos - ePos).length())
							{
								flag = false;
								image.at(np) = pixel;
								v2 = v1;
							}
						}
					}
				}
				if (timer.ms() > 16)
				{
					auto t1 = Transformer2D(Mat3x2::Scale(Window::Size().x / double(_sizeX)));
					Window::SetTitle(double(count) / double(_sizeX*_sizeX / 2));
					//tex.fill(image.getAsImage(Rect(Size(2048, 2048))));
					//tex.resize(Window::Size()).draw();
					if (!System::Update()) return;
					timer.restart();
				}
			}
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
					bool flag = true;

					for (auto& p : nodes[voronoiMap[p1.y][p1.x]].paths)
					{
						if (p.childNodeID == voronoiMap[p2.y][p2.x])
						{
							flag = false;
							break;
						}
					}

					if (flag)
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
		BinaryWriter writer(L"assets/nodemap/nodeMap.bin");
		writer.write(int(nodes.size()));
		for (int i = 0; i < int(nodes.size()); ++i)
		{
			writer.write(nodes[i].pos.ePos);
		}
		writer.write(int(paths.size()));
		for (int i = 0; i < int(paths.size()); ++i)
		{
			writer.write(paths[i]->parentNodeID);
			writer.write(paths[i]->childNodeID);
		}
		writer.close();
	}

	//ファイル書き出し
	{
		Size size(Min(8192, image.size().x), Min(8192, image.size().y));

		for (auto p : step(Size(image.size().x / size.x, image.size().y / size.y)))
		{
			image.saveImg(Rect(Point(p.x*size.x, p.y*size.y), size), Format(L"assets/VoronoiMap_", p.x, L"_", p.y, L".png"));
		}
	}

	while (System::Update());
}