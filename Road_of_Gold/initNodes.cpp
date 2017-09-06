#include"Node.h"

void	initNodes()
{
	//Nodeの読み込み
	BinaryReader reader(L"assets/nodemap/nodeMap.bin");
	if (!reader)
	{
		LOG_ERROR(L"assets/nodemap/nodeMap.binの読み込みに失敗");
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
			LOG_ERROR(L"NodeMap.binから異常な数値を検出");
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

	Log(L"nodesおよびpathsの読み込み完了");
	Log(L"総Node数:", nodes.size());
}