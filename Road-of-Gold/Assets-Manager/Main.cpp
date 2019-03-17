#include"VehicleData.h"
#include"ItemData.h"
#include"CitizenData.h"
#include"EnergyData.h"

void	LoggerFiles(const FilePath& _path)
{
	//ItemData.json
	{
		String text = U"[";
		for (int i = 0; i < int(itemData.size()); i++)
		{
			auto& d = itemData[i];
			if (i != 0) text += U",{";
			else text += U"{";
			text += U"\"Name\":\"" + d.name() + U"\",";
			text += U"\"Volume\":" + d.volumeTextBox.getText() + U",";
			text += U"\"Value\":" + d.volumeTextBox.getText() + U",";
			text += U"\"Color\":\"" + d.colorTextBox.getText() + U"\",";
			text += U"\"Icon\":\"" + d.iconTextBox.getText() + U"\"";

			text += U"}";
		}
		text += U"]";
		TextWriter(_path + U"itemData.json").write(text);
	}

	//EnergyData.json
	{
		String text = U"[";
		for (int i = 0; i < int(energyData.size()); i++)
		{
			auto& e = energyData[i];
			if (i != 0) text += U",{";
			else text += U"{";
			text += U"\"Name\":\"" + e.name() + U"\",";
			text += U"\"Icon\":\"" + e.iconTextBox.getText() + U"\"";

			text += U"}";
		}
		text += U"]";
		TextWriter(_path + U"energyData.json").write(text);
	}
	//CitizenData.json
	{
		String text = U"[";
		for (int i = 0; i < int(citizenData.size()); i++)
		{
			auto& c = citizenData[i];
			if (i != 0) text += U",{";
			else text += U"{";
			text += U"\"Name\":\"" + c.name() + U"\"";
			if (ParseInt<int32>(c.wageTextBox.getText(), Arg::radix = 10) > 0)
			{
				text += U",\"Wage\":" + c.wageTextBox.getText();
			}
			if (energyIsExist(c.needEnergyTextBox.getText()))
			{
				text += U",\"NeedEnergy\":\"" + c.needEnergyTextBox.getText() + U"\"";
			}
			if (itemIsExist(c.productNameTextBox.getText()))
			{
				text += U",\"Product\":{";
				text += U"\"ItemName\":\"" + c.productNameTextBox.getText() + U"\",";
				text += U"\"NumItem\":" + c.numProductTextBox.getText();
				text += U"}";
			}
			text += U"}";
		}
		text += U"]";
		TextWriter(_path + U"citizenData.json").write(text);
	}
	//VehicleData.json
	{
		String text = U"[";
		for (int i = 0; i < int(vehicleData.size()); i++)
		{
			auto& v = vehicleData[i];
			auto speed = ParseFloat<double>(v.speedTextBox.getText());
			auto range = ParseFloat<double>(v.rangeTextBox.getText());
			bool isShip = ParseBool(v.isShipTextBox.getText());

			if (i != 0) text += U",{";
			else text += U"{";
			text += U"\"Name\":\"" + v.name() + U"\",";
			if (speed == int(speed)) text += U"\"Speed\":" + Format(speed) + U".0,";
			else text += U"\"Speed\":" + Format(speed) + U",";
			text += U"\"Volume\":" + v.volumeTextBox.getText() + U",";
			if (range == int(range)) text += U"\"Range\":" + Format(range) + U".0,";
			else text += U"\"Range\":" + Format(range) + U",";
			text += U"\"IsShip\":" + Format(isShip) + U",";
			text += U"\"Icon\":\"" + v.iconTextBox.getText() + U"\"";
			text += U"}";
		}
		text += U"]";
		TextWriter(_path + U"vehicleData.json").write(text);
	}

}

void Main()
{
	Window::SetTitle(U"Assets-Manager");
	Window::Resize(1280, 720);


	MyApp manager;
	manager
		.add<SetItemData>(itemList[0])
		.add<SetCitizenData>(itemList[1])
		.add<SetEnergyData>(itemList[2])
		.add<SetVehicleData>(itemList[3]);
	manager.changeScene(itemList.front(), 0, false);

	{
		JSONReader json;
		json.open(U"assets/data/CitizenData.json");
		for (auto j : json.arrayView()) citizenData.emplace_back(j);
		json.open(U"assets/data/itemData.json");
		for (auto j : json.arrayView()) itemData.emplace_back(j);
		json.open(U"assets/data/energyData.json");
		for (auto j : json.arrayView()) energyData.emplace_back(j);
		json.open(U"assets/data/vehicleData.json");
		for (auto j : json.arrayView()) vehicleData.emplace_back(j);
	}

	Font	font = Font(16);

	TextBox	filePathTextBox(font, Vec2(720, 48), 120, unspecified, U"data/");

	while (System::Update())
	{
		if (!manager.update()) break;

		{
			Rect rect(720, 0, 120, 24);
			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (rect.leftClicked()) LoggerFiles(U"assets/data/");
			rect.drawFrame(2);
			font(U"ファイル出力").drawAt(rect.center());
		}
		{
			Rect rect(720, 24, 120, 24);
			if (rect.mouseOver()) rect.draw(Palette::Orange);
			rect.drawFrame(2);
			font(U"フォルダに出力").drawAt(rect.center());

			if (rect.leftClicked()) LoggerFiles(filePathTextBox.getText());
			filePathTextBox.update();
			filePathTextBox.draw();
		}


	}
}