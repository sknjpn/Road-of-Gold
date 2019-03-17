#include"Node.h"

void	initNodes()
{
	//Nodeの読み込み
	BinaryReader reader(U"assets/nodeMap/nodeMap.bin");
	if (!reader)
	{
		Logger << U"assets/nodeMap/nodeMap.binの読み込みに失敗";
		System::Exit();
		return;
	}

	int	nodesSize, pathsSize;

	//Nodeの位置の読み込み
	reader.read(nodesSize);
	nodes.reserve(nodesSize);
	for (int i = 0; i < nodesSize; ++i)
	{
		Vec3 ePos;
		reader.read(ePos);
		nodes.emplace_back(ePos);
	}

	//Pathの関係の読み込み
	reader.read(pathsSize);
	for (int i = 0; i < pathsSize; ++i)
	{
		int parentNodeID;
		int childNodeID;
		reader.read(parentNodeID);
		reader.read(childNodeID);

		if (parentNodeID < 0 || parentNodeID >= nodesSize || childNodeID < 0 || childNodeID >= nodesSize)
		{
			Logger << U"NodeMap.binから異常な数値を検出";
			System::Exit();
			return;
		}

		nodes[parentNodeID].paths.emplace_back(childNodeID);
	}

	//Lengthの適用
	for (auto& n : nodes)
	{
		for (auto& p : n.paths)
		{
			p.length = nodes[p.toID].pos.distanceFrom(n.pos);
		}
	}
	Logger << U"nodesおよびpathsの読み込み完了";
	Logger << U"総Node数:" << nodes.size();
}