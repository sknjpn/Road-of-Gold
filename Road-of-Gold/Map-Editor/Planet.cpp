#include"Planet.h"
#include"Node.h"
#include"BiomeData.h"

Planet::Planet()
	: mapTexture()
	, coverTexture()
	, coverRate(0.5)
{}

void	Planet::updateImage(Array<Node*> _nodeList)
{
	for (auto n : _nodeList)
	{
		const auto& p = (n->pos.mPos / 360_deg).movedBy(0.5, 0.25)*voronoiMap.size().x;
		Array<Point> temp;
		temp.emplace_back(int(p.x), int(p.y));
		while (!temp.isEmpty())
		{
			for (int m = 0; m < 4; m++)
			{
				Point p1 = temp.front();
				Point p2 = temp.front();
				switch (m)
				{
				case 0: p2 = { p1.x - 1,p1.y }; break;
				case 1: p2 = { p1.x + 1,p1.y }; break;
				case 2: p2 = { p1.x ,p1.y - 1 }; break;
				case 3: p2 = { p1.x ,p1.y + 1 }; break;
				}
				//例外判定
				if (p2.y < 0 || p2.y >= voronoiMap.size().y) continue;
				if (p2.x < 0) p2.x = voronoiMap.size().x - 1;
				if (p2.x >= voronoiMap.size().x) p2.x = 0;
				if (voronoiMap[p1.y][p1.x] == voronoiMap[p2.y][p2.x] && image[p2.y][p2.x] != biomeData[n->biomeType].color)
				{
					temp.emplace_back(p2);
					image[p2.y][p2.x] = biomeData[n->biomeType].color;
				}
			}
			temp.pop_front();
		}
	}
	if (!_nodeList.isEmpty()) mapTexture.fill(image);
}
void	Planet::setCoverImage()
{
	if (!DragDrop::HasNewFilePaths()) return;
	const Font font24(24);
	auto item = DragDrop::GetDroppedFilePaths().front();
	Texture tex(item.path);

	for (;;)
	{
		if (DragDrop::HasNewFilePaths())
		{
			item = DragDrop::GetDroppedFilePaths().front();
			tex = Texture(item.path);
		}
		tex.resize(Window::Size()).draw();
		Array<String> slist = { L"カバーのみを行う", L"バイオーム色が一致する場合適用", L"最も近いバイオームに設定" };
		for (int i = 0; i < int(slist.size()); i++)
		{
			Rect rect(0, i * 32, 720, 32);
			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (rect.leftClicked())
			{
				switch (i)
				{
				case 1:
				{
					Image img(item.path);
					Size s = img.size();
					Array<Node*> list;
					for (auto& n : nodes)
					{
						Vec2 p = n.pos.mPos.movedBy(180_deg, 90_deg);
						auto& c = img[int(s.y*p.y / 180_deg)][int(s.x*p.x / 360_deg)];
						for (int j = 0; j < int(biomeData.size()); j++)
						{
							if (c == biomeData[j].color)
							{
								n.biomeType = j;
								list.emplace_back(&n);
								break;
							}
						}
					}
					planet.updateImage(list);

				}
				break;
				case 2:
				{
					Image img(item.path);
					Size s = img.size();
					Array<Node*> list;
					for (auto& n : nodes)
					{
						Vec2 p = n.pos.mPos.movedBy(180_deg, 90_deg);
						auto& c = img[int(s.y*p.y / 180_deg)][int(s.x*p.x / 360_deg)];
						int min = 0;
						int target = -1;
						for (int j = 0; j < int(biomeData.size()); j++)
						{
							auto r = c.r - biomeData[j].color.r;
							auto g = c.g - biomeData[j].color.g;
							auto b = c.b - biomeData[j].color.b;
							if (target == -1 || int(r*r + g*g + b*b) < min)
							{
								min = r*r + g*g + b*b;
								target = j;
							}
						}
						if (n.biomeType != target)
						{
							list.emplace_back(&n);
							n.biomeType = target;
						}
					}
					planet.updateImage(list);
				}
				break;
				}
				coverTexture = tex;
				return;
			}
			rect.drawFrame(2);
			font24(slist[i]).drawAt(rect.center());
		}
		if (!System::Update()) return;
	}
	coverTexture = tex;
}