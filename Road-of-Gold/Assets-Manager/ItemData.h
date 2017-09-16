#pragma once
#include"united.h"

struct ItemData
{
	Font	textBoxFont = Font(16);
	TextBox	nameTextBox;
	TextBox	volumeTextBox;
	TextBox	valueTextBox;
	TextBox iconTextBox;
	TextBox colorTextBox;
	Image	iconImage;
	Texture	iconTexture;

	ItemData(JSONValue _j)
		: nameTextBox(textBoxFont, 288, 24, 120, _j[L"Name"].getString())
		, volumeTextBox(textBoxFont, 288, 24 * 3, 120, Format(_j[L"Volume"].get<int>()))
		, valueTextBox(textBoxFont, 288, 24 * 5, 120, Format(_j[L"Value"].get<int>()))
		, iconTextBox(textBoxFont, 288, 24 * 7, 360, _j[L"Icon"].getString())
		, colorTextBox(textBoxFont, 288, 24 * 9, 120, _j[L"Color"].getString())
		, iconImage(_j[L"Icon"].getString())
		, iconTexture(iconImage)
	{}
	ItemData()
		: nameTextBox(textBoxFont, 288, 24, 120, L"newItem")
		, volumeTextBox(textBoxFont, 288, 24 * 3, 120, L"1")
		, valueTextBox(textBoxFont, 288, 24 * 5, 120, L"0")
		, iconTextBox(textBoxFont, 288, 24 * 7, 360)
		, colorTextBox(textBoxFont, 288, 24 * 9, 120, L"#FF0000")
	{}
	String	name() const { return nameTextBox.getText(); }
};

Array<ItemData>	itemData;

bool	itemIsExist(const String& _itemName)
{
	return itemData.any([&_itemName](const ItemData& i) { return i.name() == _itemName; });
}
struct SetItemData : MyApp::Scene
{
	Font	font16 = Font(16);


	SetItemData(const InitData& init)
		: IScene(init)
	{}

	void update() override
	{
		//左メニュー
		for (int i = 0; i < int(itemList.size()); i++)
		{
			Rect rect(0, i * 24, 24 * 6, 24);
			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (i == 0) rect.draw(Palette::Red);
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

		//既存のアイテム
		for (int i = 0; i < int(itemData.size()); i++)
		{
			Rect rect(24 * 6, i * 24, 24 * 5, 24);

			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (rect.leftClicked()) getData().selectedItemType = i;
			if (i == getData().selectedItemType) rect.draw(Palette::Red);
			rect.drawFrame(2);

			if (i < int(itemData.size())) font16(itemData[i].name()).draw(rect.movedBy(4, 0));
		}

		//新アイテム追加
		{
			Rect rect(144, int(itemData.size()) * 24, 24 * 5, 24);

			rect.draw(Palette::Gray);
			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (rect.leftClicked())
			{
				getData().selectedItemType = int(itemData.size());
				itemData.emplace_back();
			}
			rect.drawFrame(2);

			font16(L"新アイテム追加").draw(rect.movedBy(4, 0));
		}

		if (itemData.isEmpty()) return;

		//上下キー対応
		{
			if (KeyUp.down() && getData().selectedItemType > 0) --getData().selectedItemType;
			if (KeyDown.down() && getData().selectedItemType < int(itemData.size() - 1)) ++getData().selectedItemType;
		}

		//Tabキー対応
		if(KeyTab.down())
		{
			auto& i = itemData[getData().selectedItemType];
			if (i.nameTextBox.isActive())
			{
				i.nameTextBox.setActive(false);
				i.volumeTextBox.setActive(true);
			}
			else if (i.volumeTextBox.isActive())
			{
				i.volumeTextBox.setActive(false);
				i.valueTextBox.setActive(true);
			}
			else if (i.valueTextBox.isActive())
			{
				i.valueTextBox.setActive(false);
				i.iconTextBox.setActive(true);
			}
			else if (i.iconTextBox.isActive())
			{
				i.iconTextBox.setActive(false);
				i.colorTextBox.setActive(true);
			}
			else if (i.colorTextBox.isActive())
			{
				i.colorTextBox.setActive(false);
				i.nameTextBox.setActive(true);
			}
			else i.nameTextBox.setActive(true);
		}

		//アイテムの更新＆描画
		{
			auto& i = itemData[getData().selectedItemType];

			font16(L"アイテム名").drawAt(348, 12);
			i.nameTextBox.update();
			i.nameTextBox.draw();

			font16(L"アイテム容量").drawAt(348, 60);
			i.volumeTextBox.update();
			i.volumeTextBox.draw();
			i.volumeTextBox.setText(Format(ParseInt<int32>(i.volumeTextBox.getText(), Arg::radix = 10)));

			font16(L"アイテム価値").drawAt(348, 108);
			i.valueTextBox.update();
			i.valueTextBox.draw();
			i.valueTextBox.setText(Format(ParseInt<int32>(i.valueTextBox.getText(), Arg::radix = 10)));

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
			Rect(432, 24, 128, 128).draw(DragDrop::DragOver() ? Palette::Gray : Color(0, 0)).drawFrame(4, Color(i.colorTextBox.getText()));
			if (i.iconTexture) i.iconTexture.resize(128, 128).draw(432, 24);

			font16(L"アイテムカラー").drawAt(348, 204);
			i.colorTextBox.update();
			i.colorTextBox.draw();

		}

		{
			Rect rect(720, 72, 120, 24);
			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (rect.leftClicked())
			{
				itemData.erase(itemData.begin() + getData().selectedItemType);
				if (getData().selectedItemType == int(itemData.size()) && getData().selectedItemType > 0) getData().selectedItemType--;
			}
			rect.drawFrame(2);
			font16(L"項目の削除").drawAt(rect.center());
		}
	}
};