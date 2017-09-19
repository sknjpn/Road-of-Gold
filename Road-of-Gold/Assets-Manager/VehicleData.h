#pragma once
#include"united.h"

struct VehicleData
{
	Font	textBoxFont = Font(16);
	TextBox	nameTextBox;
	TextBox	speedTextBox;
	TextBox	volumeTextBox;
	TextBox iconTextBox;
	TextBox rangeTextBox;
	TextBox isShipTextBox;
	Image	iconImage;
	Texture	iconTexture;

	VehicleData(JSONValue _j)
		: nameTextBox(textBoxFont, 288, 24, 120, _j[L"Name"].getString())
		, speedTextBox(textBoxFont, 288, 24 * 3, 120, Format(_j[L"Speed"].get<double>()))
		, volumeTextBox(textBoxFont, 288, 24 * 5, 120, Format(_j[L"Volume"].get<int>()))
		, iconTextBox(textBoxFont, 288, 24 * 7, 360, _j[L"Icon"].getString())
		, rangeTextBox(textBoxFont, 288, 24 * 9, 120, Format(_j[L"Range"].get<double>()))
		, isShipTextBox(textBoxFont, 432, 24 * 9, 120, Format(_j[L"IsShip"].get<bool>()))
		, iconImage(_j[L"Icon"].getString())
		, iconTexture(iconImage)
	{}
	VehicleData()
		: nameTextBox(textBoxFont, 288, 24, 120, L"newVehicle")
		, speedTextBox(textBoxFont, 288, 24 * 3, 120, L"1.0")
		, volumeTextBox(textBoxFont, 288, 24 * 5, 120, L"100")
		, iconTextBox(textBoxFont, 288, 24 * 7, 360)
		, rangeTextBox(textBoxFont, 288, 24 * 9, 120, L"10.0")
		, isShipTextBox(textBoxFont, 432, 24 * 9, 120, L"true")
	{}
	String	name() const { return nameTextBox.getText(); }
};

Array<VehicleData>	vehicleData;

struct SetVehicleData : MyApp::Scene
{
	Font	font16 = Font(16);

	SetVehicleData(const InitData& init)
		: IScene(init)
	{}

	void update() override
	{
		//左メニュー
		for (int i = 0; i < int(itemList.size()); i++)
		{
			Rect rect(0, i * 24, 24 * 6, 24);
			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (i == 3) rect.draw(Palette::Red);
			if (rect.leftClicked())
			{
				getData().selectedSceneID = i;
				changeScene(itemList[i], 0ms, false);
			}
			rect.drawFrame(2);
			font16(itemList[i]).draw(rect.movedBy(4, 0));
		}

		//左メニューキー操作
		if(false)
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
		for (int i = 0; i < int(vehicleData.size()); i++)
		{
			Rect rect(24 * 6, i * 24, 24 * 5, 24);

			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (rect.leftClicked()) getData().selectedVehicleType = i;
			if (i == getData().selectedVehicleType) rect.draw(Palette::Red);
			rect.drawFrame(2);

			if (i < int(vehicleData.size())) font16(vehicleData[i].name()).draw(rect.movedBy(4, 0));
		}

		//新アイテム追加
		{
			Rect rect(144, int(vehicleData.size()) * 24, 24 * 5, 24);

			rect.draw(Palette::Gray);
			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (rect.leftClicked())
			{
				getData().selectedVehicleType = int(vehicleData.size());
				vehicleData.emplace_back();
			}
			rect.drawFrame(2);

			font16(L"新アイテム追加").draw(rect.movedBy(4, 0));
		}

		if (vehicleData.isEmpty()) return;

		//上下キー対応
		{
			if (KeyUp.down() && getData().selectedVehicleType > 0) --getData().selectedVehicleType;
			if (KeyDown.down() && getData().selectedVehicleType < int(vehicleData.size() - 1)) ++getData().selectedVehicleType;
		}

		//Tabキー対応
		if (KeyTab.down())
		{
			auto& i = vehicleData[getData().selectedVehicleType];
			if (i.nameTextBox.isActive())
			{
				i.nameTextBox.setActive(false);
				i.speedTextBox.setActive(true);
			}
			else if (i.speedTextBox.isActive())
			{
				i.speedTextBox.setActive(false);
				i.volumeTextBox.setActive(true);
			}
			else if (i.volumeTextBox.isActive())
			{
				i.volumeTextBox.setActive(false);
				i.iconTextBox.setActive(true);
			}
			else if (i.iconTextBox.isActive())
			{
				i.iconTextBox.setActive(false);
				i.rangeTextBox.setActive(true);
			}
			else if (i.rangeTextBox.isActive())
			{
				i.rangeTextBox.setActive(false);
				i.isShipTextBox.setActive(true);
			}
			else if (i.isShipTextBox.isActive())
			{
				i.isShipTextBox.setActive(false);
				i.nameTextBox.setActive(true);
			}
			else i.nameTextBox.setActive(true);
		}
		
		//アイテムの更新＆描画
		{
			auto& i = vehicleData[getData().selectedVehicleType];

			font16(L"ユニット名").drawAt(348, 12);
			i.nameTextBox.update();
			i.nameTextBox.draw();

			font16(L"航行速度(小数)").drawAt(348, 60);
			i.speedTextBox.update();
			i.speedTextBox.draw();

			font16(L"積載容量").drawAt(348, 108);
			i.volumeTextBox.update();
			i.volumeTextBox.draw();
			i.volumeTextBox.setText(Format(ParseInt<int32>(i.volumeTextBox.getText(), Arg::radix = 10)));

			font16(L"アイコンファイル").drawAt(348, 156);
			auto beforeFilePath = i.iconTextBox.getText();
			i.iconTextBox.update();
			i.iconTextBox.draw();
			if (DragDrop::HasNewFilePaths()) i.iconTextBox.setText(DragDrop::GetDroppedFilePaths().front().path);
			if (beforeFilePath != i.iconTextBox.getText())
			{
				i.iconImage = Image(i.iconTextBox.getText());
				i.iconTexture = Texture(i.iconImage);
			}
			Rect(432, 24, 128, 128).draw(DragDrop::DragOver() ? Palette::Gray : Color(0, 0)).drawFrame(4);
			if (i.iconTexture) i.iconTexture.resize(128, 128).draw(432, 24);

			font16(L"航続距離(小数)").drawAt(348, 204);
			i.rangeTextBox.update();
			i.rangeTextBox.draw();

			font16(L"船かどうか(bool)").drawAt(492, 204);
			i.isShipTextBox.update();
			i.isShipTextBox.draw();

		}

		{
			Rect rect(720, 72, 120, 24);
			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (rect.leftClicked())
			{
				vehicleData.erase(vehicleData.begin() + getData().selectedVehicleType);
				if (getData().selectedVehicleType == int(vehicleData.size()) && getData().selectedVehicleType > 0) getData().selectedVehicleType--;
			}
			rect.drawFrame(2);
			font16(L"項目の削除").drawAt(rect.center());
		}
	}
};