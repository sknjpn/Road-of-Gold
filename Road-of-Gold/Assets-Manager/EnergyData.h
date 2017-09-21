#pragma once
#include"united.h"

struct EnergyData
{
	Font	textBoxFont = Font(16);
	TextBox	nameTextBox;
	TextBox iconTextBox;
	Image	iconImage;
	Texture	iconTexture;

	EnergyData(JSONValue _j)
		: nameTextBox(textBoxFont, 288, 24, 120, _j[L"Name"].getString())
		, iconTextBox(textBoxFont, 288, 24 * 7, 360, _j[L"Icon"].getString())
		, iconImage(_j[L"Icon"].getString())
		, iconTexture(iconImage)
	{}
	EnergyData()
		: nameTextBox(textBoxFont, 288, 24, 120, L"newItem")
		, iconTextBox(textBoxFont, 288, 24 * 7, 360)
	{}
	String	name() const { return nameTextBox.getText(); }
};

Array<EnergyData>	energyData;

bool	energyIsExist(const String& _energyName)
{
	return energyData.any([&_energyName](const EnergyData& i) { return i.name() == _energyName; });
}
struct SetEnergyData : MyApp::Scene
{
	Font	font16 = Font(16);


	SetEnergyData(const InitData& init)
		: IScene(init)
	{}

	void update() override
	{
		//左メニュー
		for (int i = 0; i < int(itemList.size()); i++)
		{
			Rect rect(0, i * 24, 24 * 6, 24);
			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (i == 2) rect.draw(Palette::Red);
			if (rect.leftClicked())
			{
				getData().selectedSceneID = i;
				changeScene(itemList[i], 0ms, false);
			}
			rect.drawFrame(2);
			font16(itemList[i]).draw(rect.movedBy(4, 0));
		}

		//左メニューキー操作
		if (false)
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


		//既存のアイテム
		for (int i = 0; i < int(energyData.size()); i++)
		{
			Rect rect(24 * 6, i * 24, 24 * 5, 24);

			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (rect.leftClicked()) getData().selectedEnergyType = i;
			if (i == getData().selectedEnergyType) rect.draw(Palette::Red);
			rect.drawFrame(2);

			if (i < int(energyData.size())) font16(energyData[i].name()).draw(rect.movedBy(4, 0));
		}

		//新アイテム追加
		{
			Rect rect(144, int(energyData.size()) * 24, 24 * 5, 24);

			rect.draw(Palette::Gray);
			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (rect.leftClicked())
			{
				getData().selectedEnergyType = int(energyData.size());
				energyData.emplace_back();
			}
			rect.drawFrame(2);

			font16(L"新資源追加").draw(rect.movedBy(4, 0));
		}

		if (energyData.isEmpty()) return;

		//上下キー対応
		{
			if (KeyUp.down() && getData().selectedEnergyType > 0) --getData().selectedEnergyType;
			if (KeyDown.down() && getData().selectedEnergyType < int(energyData.size() - 1)) ++getData().selectedEnergyType;
		}

		//Tabキー対応
		if (KeyTab.down())
		{
			auto& e = energyData[getData().selectedEnergyType];
			if (e.nameTextBox.isActive())
			{
				e.nameTextBox.setActive(false);
				e.iconTextBox.setActive(true);
			}
			else if (e.iconTextBox.isActive())
			{
				e.iconTextBox.setActive(false);
				e.nameTextBox.setActive(true);
			}
			else e.nameTextBox.setActive(true);
		}

		//クリップボード
		if (KeyControl.pressed() && KeyV.down())
		{
			auto& e = energyData[getData().selectedEnergyType];
			String string;

			Clipboard::GetText(string);
			if (string)
			{
				if (e.nameTextBox.isActive()) e.nameTextBox.setText(string);
				if (e.iconTextBox.isActive()) e.iconTextBox.setText(string);
			}
		}

		//アイテムの更新＆描画
		{
			auto& e = energyData[getData().selectedEnergyType];

			font16(L"資源名").drawAt(348, 12);
			e.nameTextBox.update();
			e.nameTextBox.draw();

			font16(L"アイコンファイル").drawAt(348, 156);
			auto beforeFilePath = e.iconTextBox.getText();
			e.iconTextBox.update();
			e.iconTextBox.draw();
			if (DragDrop::HasNewFilePaths()) e.iconTextBox.setText(DragDrop::GetDroppedFilePaths().front().path);
			if (beforeFilePath != e.iconTextBox.getText())
			{
				e.iconImage = Image(e.iconTextBox.getText());
				e.iconTexture = Texture(e.iconImage);
			}
			Rect(432, 24, 128, 128).draw(DragDrop::DragOver() ? Palette::Gray : Color(0, 0)).drawFrame(4);
			if (e.iconTexture) e.iconTexture.resize(128, 128).draw(432, 24);
		}

		{
			Rect rect(720, 72, 120, 24);
			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (rect.leftClicked())
			{
				energyData.erase(energyData.begin() + getData().selectedEnergyType);
				if (getData().selectedEnergyType == int(energyData.size()) && getData().selectedEnergyType > 0) getData().selectedEnergyType--;
			}
			rect.drawFrame(2);
			font16(L"項目の削除").drawAt(rect.center());
		}
	}
};