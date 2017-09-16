#pragma once
#include"united.h"

struct CitizenData
{
	Font	textBoxFont = Font(16);
	TextBox	nameTextBox;
	TextBox	wageTextBox;
	TextBox	needEnergyTextBox;
	TextBox productNameTextBox;
	TextBox numProductTextBox;

	CitizenData(JSONValue _j)
		: nameTextBox(textBoxFont, 288, 24, 120, _j[L"Name"].getString())
		, wageTextBox(textBoxFont, 288, 24 * 3, 120, Format(_j[L"Wage"].getOr<int>(0)))
		, needEnergyTextBox(textBoxFont, 288, 24 * 5, 120, _j[L"NeedEnergy"].getString())
		, productNameTextBox(textBoxFont, 288, 24 * 7, 120, _j[L"Product"][L"ItemName"].getOr<String>(L""))
		, numProductTextBox(textBoxFont, 288, 24 * 9, 120, Format(_j[L"Product"][L"NumItem"].getOr<int>(0)))
	{}
	CitizenData()
		: nameTextBox(textBoxFont, 288, 24, 120, L"newCitizen")
		, wageTextBox(textBoxFont, 288, 24 * 3, 120, L"0")
		, needEnergyTextBox(textBoxFont, 288, 24 * 5, 120, L"")
		, productNameTextBox(textBoxFont, 288, 24 * 7, 120, L"")
		, numProductTextBox(textBoxFont, 288, 24 * 9, 120, L"0")
	{}
	String	name() const { return nameTextBox.getText(); }
};

Array<CitizenData>	citizenData;

struct SetCitizenData : MyApp::Scene
{
	Font	font16 = Font(16);


	SetCitizenData(const InitData& init)
		: IScene(init)
	{}

	void update() override
	{
		//左メニュー
		for (int i = 0; i < int(itemList.size()); i++)
		{
			Rect rect(0, i * 24, 24 * 6, 24);
			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (i == 1) rect.draw(Palette::Red);
			if (rect.leftClicked())
			{
				getData().selectedSceneID = i;
				changeScene(itemList[i], 0ms, false);
			}
			rect.drawFrame(2);
			font16(itemList[i]).draw(rect.movedBy(4, 0));
		}

		//左メニューキー操作
		{
			if (KeyW.down() && getData().selectedSceneID > 0)
			{
				getData().selectedSceneID--;
				changeScene(itemList[getData().selectedSceneID], 0ms, false);
			}
			if (KeyS.down() && getData().selectedSceneID < int(itemList.size() - 1))
			{
				getData().selectedSceneID++;
				changeScene(itemList[getData().selectedSceneID], 0ms, false);
			}
		}

		//既存の市民
		for (int i = 0; i < int(citizenData.size()); i++)
		{
			Rect rect(24 * 6, i * 24, 24 * 5, 24);

			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (rect.leftClicked()) getData().selectedCitizenType = i;
			if (i == getData().selectedCitizenType) rect.draw(Palette::Red);
			rect.drawFrame(2);

			font16(citizenData[i].name()).draw(rect.movedBy(4, 0));
		}

		//新市民追加
		{
			Rect rect(144, int(citizenData.size()) * 24, 24 * 5, 24);

			rect.draw(Palette::Gray);
			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (rect.leftClicked())
			{
				getData().selectedCitizenType = int(citizenData.size());
				citizenData.emplace_back();
			}
			rect.drawFrame(2);

			font16(L"新市民追加").draw(rect.movedBy(4, 0));
		}

		if (citizenData.isEmpty()) return;

		//上下キー対応
		{
			if (KeyUp.down() && getData().selectedCitizenType > 0) --getData().selectedCitizenType;
			if (KeyDown.down() && getData().selectedCitizenType < int(citizenData.size() - 1)) ++getData().selectedCitizenType;
		}

		//Tabキー対応
		if (KeyTab.down())
		{
			auto& c = citizenData[getData().selectedCitizenType];
			if (c.nameTextBox.isActive())
			{
				c.nameTextBox.setActive(false);
				c.wageTextBox.setActive(true);
			}
			else if (c.wageTextBox.isActive())
			{
				c.wageTextBox.setActive(false);
				c.needEnergyTextBox.setActive(true);
			}
			else if (c.needEnergyTextBox.isActive())
			{
				c.needEnergyTextBox.setActive(false);
				c.productNameTextBox.setActive(true);
			}
			else if (c.productNameTextBox.isActive())
			{
				c.productNameTextBox.setActive(false);
				c.numProductTextBox.setActive(true);
			}
			else if (c.numProductTextBox.isActive())
			{
				c.numProductTextBox.setActive(false);
				c.nameTextBox.setActive(true);
			}
			else c.nameTextBox.setActive(true);
		}

		//市民の更新＆描画
		{
			auto& c = citizenData[getData().selectedCitizenType];

			font16(L"市民名").drawAt(348, 12);
			c.nameTextBox.update();
			c.nameTextBox.draw();

			font16(L"賃金").drawAt(348, 60);
			c.wageTextBox.update();
			c.wageTextBox.draw();
			c.wageTextBox.setText(Format(Max(0, ParseInt<int32>(c.wageTextBox.getText(), Arg::radix = 10))));

			font16(L"必要資源").drawAt(348, 108);
			c.needEnergyTextBox.update();
			c.needEnergyTextBox.draw(energyIsExist(c.needEnergyTextBox.getText()) ? Palette::White : Palette::Red);

			font16(L"生産アイテム名").drawAt(348, 156);
			c.productNameTextBox.update();
			c.productNameTextBox.draw(itemIsExist(c.productNameTextBox.getText()) ? Palette::White : Palette::Red);

			font16(L"アイテム生産数").drawAt(348, 204);
			c.numProductTextBox.update();
			c.numProductTextBox.draw();

		}

		{
			Rect rect(720, 72, 120, 24);
			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (rect.leftClicked())
			{
				citizenData.erase(citizenData.begin() + getData().selectedCitizenType);

				if (getData().selectedCitizenType == int(citizenData.size()) && getData().selectedCitizenType > 0) getData().selectedCitizenType--;
			}
			rect.drawFrame(2);
			font16(L"項目の削除").drawAt(rect.center());
		}
	}
};