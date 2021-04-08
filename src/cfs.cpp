#include "cfs.h"

Vector CFS::img_to_world_pos(Vector imgV)
{
	imgV.x = imgV.x -svgSize.x/2.f;
	imgV.y = svgSize.y - imgV.y;
	imgV.y = imgV.y - svgSize.y/2.f;
	return imgV;
}
Vector CFS::world_to_img_pos(Vector worldV)
{
	worldV.y += svgSize.y/2.f;
	worldV.y = svgSize.y - worldV.y;
	worldV.x += svgSize.x/2.f;
	return worldV;
}
Vector CFS::world_to_screen_pos(Vector worlV)
{
	worlV.y += svgSize.y/2.f;
	worlV.y = svgSize.y - worlV.y;
	worlV.x += svgSize.x/2.f;
	return vec_v(worlV.x/(float)svgSize.x * (float)SCREEN_W, worlV.y/(float)svgSize.y * (float)SCREEN_H);
}

void CFS::generate_target_eq(std::string fileName)
{
	targetEq.clear();
	std::ifstream inFile;
	// std::cout << "res/saves/" + fileName << std::endl;
	inFile.open("res/saves/" + fileName);
	if (!inFile)
	{
		throw 0;
	}
	std::vector<std::string> myLines;

	std::string str;
	while (std::getline(inFile, str))
	{
	    if(str.size() > 0)
	        myLines.push_back(str);
	}
	std::string svgPath = myLines[0];

	// std::cout << svgPath << std::endl;


	std::vector<std::string> words;
	std::string currentWord;
	for (unsigned i = 0; i < svgPath.length(); ++i)
	{
		if (svgPath[i] == ' ')
		{
			words.push_back(currentWord);
			currentWord = "";
			continue;
		}
		currentWord += svgPath[i];
	}
	words.push_back(currentWord);
	std::vector<instruction> instructionList;
	instruction currentInstruction;
	char myChar;
	for (unsigned i = 0; i < words.size(); ++i)
	{
		myChar = words[i][0];
		if (std::isalpha(myChar))
		{
			if (i > 0)
			{
				instructionList.push_back(currentInstruction);
			}
			currentInstruction.type = myChar;
			currentInstruction.positions.clear();
			continue;
		}
		currentInstruction.positions.push_back(std::stof(words[i]));
	}
	instructionList.push_back(currentInstruction);

	Vector cursor = vec_v(0,0);
	Vector lastControlPointPos = vec_v(0,0);
	for (unsigned i = 0; i < instructionList.size(); ++i)
	{
		equation currentEq;
		if (instructionList[i].type == 'M' or instructionList[i].type == 'm')
		{
			Vector currentWorldPos = img_to_world_pos(vec_v(instructionList[i].positions[0], instructionList[i].positions[1]));
			cursor = currentWorldPos;
			lastControlPointPos = currentWorldPos;
			continue;
		}
		if (instructionList[i].type == 'L')
		{
			Vector currentWorldPos = img_to_world_pos(vec_v(instructionList[i].positions[0], instructionList[i].positions[1]));
			std::complex<float> startComplex(cursor.x, cursor.y);
			std::complex<float> endComplex(currentWorldPos.x, currentWorldPos.y);
			currentEq.c = endComplex - startComplex;
			currentEq.d = startComplex;
			targetEq.push_back(currentEq);
			cursor = currentWorldPos;
			lastControlPointPos = currentWorldPos;
			continue;
		}
		if (instructionList[i].type == 'l')
		{
			Vector currentWorldPos = img_to_world_pos(vec_v(instructionList[i].positions[0], instructionList[i].positions[1]));
			std::complex<float> startComplex(cursor.x, cursor.y);
			Vector endVec = img_to_world_pos(world_to_img_pos(cursor) + world_to_img_pos(currentWorldPos));
			std::complex<float> endComplex(endVec.x,  endVec.y);
			currentEq.c = endComplex - startComplex;
			currentEq.d = startComplex;
			targetEq.push_back(currentEq);
			cursor = endVec;
			lastControlPointPos = cursor;
			continue;
		}
		if (instructionList[i].type == 'h')
		{
			std::complex<float> startComplex(cursor.x, cursor.y);
			Vector endVec = img_to_world_pos(world_to_img_pos(cursor) + vec_v(instructionList[i].positions[0], 0));
			std::complex<float> endComplex(endVec.x,  endVec.y);
			currentEq.c = endComplex - startComplex;
			currentEq.d = startComplex;
			targetEq.push_back(currentEq);
			cursor = endVec;
			lastControlPointPos = cursor;
			continue;
		}
		if (instructionList[i].type == 'H')
		{
			std::complex<float> startComplex(cursor.x, cursor.y);
			Vector endVec = img_to_world_pos(vec_v(instructionList[i].positions[0], cursor.y));
			std::complex<float> endComplex(endVec.x,  endVec.y);
			currentEq.c = endComplex - startComplex;
			currentEq.d = startComplex;
			targetEq.push_back(currentEq);
			cursor = endVec;
			lastControlPointPos = cursor;
			continue;
		}
		if (instructionList[i].type == 'C')
		{
			Vector currentWorldPos = img_to_world_pos(vec_v(instructionList[i].positions[0], instructionList[i].positions[1]));
			Vector vecTwo = img_to_world_pos(vec_v(instructionList[i].positions[2], instructionList[i].positions[3]));
			Vector vecThree = img_to_world_pos(vec_v(instructionList[i].positions[4], instructionList[i].positions[5]));
			std::complex<float> pZero(cursor.x, cursor.y);
			std::complex<float> pOne(currentWorldPos.x, currentWorldPos.y);
			std::complex<float> pTwo(vecTwo.x, vecTwo.y);
			std::complex<float> pThree(vecThree.x, vecThree.y);

			currentEq.a = -pZero + std::complex<float>(3,0) * pOne - std::complex<float>(3,0) * pTwo + pThree;
			currentEq.b = std::complex<float>(3,0)*pZero - std::complex<float>(6,0) * pOne + std::complex<float>(3,0) * pTwo;
			currentEq.c = -std::complex<float>(3,0) * pZero + std::complex<float>(3,0) * pOne;
			currentEq.d = pZero;
			targetEq.push_back(currentEq);
			cursor = vecThree;
			lastControlPointPos = currentWorldPos;
			continue;
		}
		if (instructionList[i].type == 'c')
		{
			Vector vecOne = img_to_world_pos(world_to_img_pos(cursor) + vec_v(instructionList[i].positions[0], instructionList[i].positions[1]));
			Vector vecTwo = img_to_world_pos(world_to_img_pos(cursor) + vec_v(instructionList[i].positions[2], instructionList[i].positions[3]));
			Vector vecThree = img_to_world_pos(world_to_img_pos(cursor) + vec_v(instructionList[i].positions[4], instructionList[i].positions[5]));
			std::complex<float> pZero(cursor.x, cursor.y);
			std::complex<float> pOne(vecOne.x, vecOne.y);
			std::complex<float> pTwo(vecTwo.x, vecTwo.y);
			std::complex<float> pThree(vecThree.x, vecThree.y);

			currentEq.a = -pZero + std::complex<float>(3,0) * pOne - std::complex<float>(3,0) * pTwo + pThree;
			currentEq.b = std::complex<float>(3,0)*pZero - std::complex<float>(6,0) * pOne + std::complex<float>(3,0) * pTwo;
			currentEq.c = -std::complex<float>(3,0) * pZero + std::complex<float>(3,0) * pOne;
			currentEq.d = pZero;
			targetEq.push_back(currentEq);
			cursor = vecThree;
			lastControlPointPos = vecOne;
			continue;
		}
		if (instructionList[i].type == 's')
		{
			Vector vecTwo = img_to_world_pos(world_to_img_pos(cursor) + vec_v(instructionList[i].positions[0], instructionList[i].positions[1]));
			Vector vecThree = img_to_world_pos(world_to_img_pos(cursor) + vec_v(instructionList[i].positions[2], instructionList[i].positions[3]));
			std::complex<float> pZero(cursor.x, cursor.y);
			std::complex<float> pOne(lastControlPointPos.x, lastControlPointPos.y);
			std::complex<float> pTwo(vecTwo.x, vecTwo.y);
			std::complex<float> pThree(vecThree.x, vecThree.y);

			currentEq.a = -pZero + std::complex<float>(3,0) * pOne - std::complex<float>(3,0) * pTwo + pThree;
			currentEq.b = std::complex<float>(3,0)*pZero - std::complex<float>(6,0) * pOne + std::complex<float>(3,0) * pTwo;
			currentEq.c = -std::complex<float>(3,0) * pZero + std::complex<float>(3,0) * pOne;
			currentEq.d = pZero;
			targetEq.push_back(currentEq);
			cursor = vecThree;
			continue;
		}

	}
	eqNB = targetEq.size();
}

void CFS::set_middle_point()
{
	centerPos = 0;
	std::complex<float> sum = 0;
	int n = 0;
	for (float i = 0.f; i < 1.f; i+=INT_DT)
	{
		sum += apply_target_equation(i);
		n++;
	}
	centerPos = sum/std::complex<float>(n, 0);
}


std::complex<float> CFS::apply_target_equation(float t)
{
	int equationID = (int)(t * (float)eqNB);
	t = t * (float)eqNB - (float)equationID;
	return targetEq[equationID].d + targetEq[equationID].c * t + targetEq[equationID].b * t * t + targetEq[equationID].a * t * t * t - centerPos;
}

std::complex<float> CFS::calculate_nth_term(int n)
{
	std::complex<float> inetgralSum = 0;
	for (float i = 0.f; i < 1.f; i+=INT_DT)
	{
		std::complex<float> rectArea = std::complex<float>(INT_DT,0) * apply_target_equation(i) * std::exp(std::complex<float>((float)n*2.f*PI*i)*std::complex<float>(0,1.f));
		inetgralSum += rectArea;
	}
	return inetgralSum;
}

std::complex<float> CFS::apply_cfs_equation_at_n(float t, int n)
{
	return std::exp(std::complex<float>((float)(coef[n].index) * 2.f * PI * t , 0.f) * std::complex<float>(0.f , 1.f)) * coef[n].val;
}

std::complex<float> CFS::apply_cfs_equation(float t)
{
	std::complex<float> result(0, 0);
	for (int i = 0; i < circlesNB; ++i)
	{
		result += apply_cfs_equation_at_n(t, i);
	}
	return result;
}

void CFS::calculate_coef()
{
	coef.clear();
	int currentN = -circlesNB/2;
	for (int i = 0; i < circlesNB; ++i)
	{
		coeficient currentCoef;
		currentCoef.val = calculate_nth_term(currentN);
		currentCoef.index = currentN;
		currentN++;
		coef.push_back(currentCoef);
	}
}

void CFS::sort_coef()
{
	sort(coef.begin(),coef.end(),[](const coeficient& a, const coeficient& b)
	{
	   return std::abs(a.val) > std::abs(b.val);
	});
}