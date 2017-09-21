#include"VehicleData.h"
#include"ItemData.h"
#include"CitizenData.h"
#include"EnergyData.h"

void	outputFiles(const FilePath& _path)
{
	//ItemData.json
	{
		String text = L"[";
		for (int i = 0; i < int(itemData.size()); i++)
		{
			auto& d = itemData[i];
			if (i != 0) text += L",{";
			else text += L"{";
			text += L"\"Name\":\"" + d.name() + L"\",";
			text += L"\"Volume\":" + d.volumeTextBox.getText() + L",";
			text += L"\"Value\":" + d.volumeTextBox.getText() + L",";
			text += L"\"Color\":\"" + d.colorTextBox.getText() + L"\",";
			text += L"\"Icon\":\"" + d.iconTextBox.getText() + L"\"";

			text += L"}";
		}
		text += L"]";
		TextWriter(_path + L"itemData.json").write(text);
	}

	//EnergyData.json
	{
		String text = L"[";
		for (int i = 0; i < int(energyData.size()); i++)
		{
			auto& e = energyData[i];
			if (i != 0) text += L",{";
			else text += L"{";
			text += L"\"Name\":\"" + e.name() + L"\",";
			text += L"\"Icon\":\"" + e.iconTextBox.getText() + L"\"";

			text += L"}";
		}
		text += L"]";
		TextWriter(_path + L"energyData.json").write(text);
	}
	//CitizenData.json
	{
		String text = L"[";
		for (int i = 0; i < int(citizenData.size()); i++)
		{
			auto& c = citizenData[i];
			if (i != 0) text += L",{";
			else text += L"{";
			text += L"\"Name\":\"" + c.name() + L"\"";
			if (ParseInt<int32>(c.wageTextBox.getText(), Arg::radix = 10) > 0)
			{
				text += L",\"Wage\":" + c.wageTextBox.getText();
			}
			if (energyIsExist(c.needEnergyTextBox.getText()))
			{
				text += L",\"NeedEnergy\":\"" + c.needEnergyTextBox.getText() + L"\"";
			}
			if (itemIsExist(c.productNameTextBox.getText()))
			{
				text += L",\"Product\":{";
				text += L"\"ItemName\":\"" + c.productNameTextBox.getText() + L"\",";
				text += L"\"NumItem\":" + c.numProductTextBox.getText();
				text += L"}";
			}
			text += L"}";
		}
		text += L"]";
		TextWriter(_path + L"citizenData.json").write(text);
	}
	//VehicleData.json
	{
		String text = L"[";
		for (int i = 0; i < int(vehicleData.size()); i++)
		{
			auto& v = vehicleData[i];
			auto speed = ParseFloat<double>(v.speedTextBox.getText());
			auto range = ParseFloat<double>(v.rangeTextBox.getText());
			bool isShip = ParseBool(v.isShipTextBox.getText());

			if (i != 0) text += L",{";
			else text += L"{";
			text += L"\"Name\":\"" + v.name() + L"\",";
			if (speed == int(speed)) text += L"\"Speed\":" + Format(speed) + L".0,";
			else text += L"\"Speed\":" + Format(speed) + L",";
			text += L"\"Volume\":" + v.volumeTextBox.getText() + L",";
			if (range == int(range)) text += L"\"Range\":" + Format(range) + L".0,";
			else text += L"\"Range\":" + Format(range) + L",";
			text += L"\"IsShip\":" + Format(isShip) + L",";
			text += L"\"Icon\":\"" + v.iconTextBox.getText() + L"\"";
			text += L"}";
		}
		text += L"]";
		TextWriter(_path + L"vehicleData.json").write(text);
	}

}

void Main()
{
	Window::SetTitle(L"Assets-Manager");
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
		json.open(L"assets/data/CitizenData.json");
		for (auto j : json.arrayView()) citizenData.emplace_back(j);
		json.open(L"assets/data/itemData.json");
		for (auto j : json.arrayView()) itemData.emplace_back(j);
		json.open(L"assets/data/energyData.json");
		for (auto j : json.arrayView()) energyData.emplace_back(j);
		json.open(L"assets/data/vehicleData.json");
		for (auto j : json.arrayView()) vehicleData.emplace_back(j);
	}

	Font	font = Font(16);

	TextBox	filePathTextBox(font, 720, 48, 120, L"data/");

	while (System::Update())
	{
		if (!manager.update()) break;

		{
			Rect rect(720, 0, 120, 24);
			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if (rect.leftClicked()) outputFiles(L"assets/data/");
			rect.drawFrame(2);
			font(L"ファイル出力").drawAt(rect.center());
		}
		{
			Rect rect(720, 24, 120, 24);
			if (rect.mouseOver()) rect.draw(Palette::Orange);
			rect.drawFrame(2);
			font(L"フォルダに出力").drawAt(rect.center());

			if (rect.leftClicked()) outputFiles(filePathTextBox.getText());
			filePathTextBox.update();
			filePathTextBox.draw();
		}


	}
}