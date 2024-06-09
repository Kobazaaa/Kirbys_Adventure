#include "pch.h"
#include "Level.h"
#include "Kirby.h"
#include "ViewFade.h"
#include "LevelManager.h"
#include <cassert>
#include <sstream>
#include <fstream>
#include <iostream>

LevelManager::LevelManager(const std::string& filePath, Kirby* kirby, Camera* camera)
	: m_pKirby{ kirby }
	, m_pCamera{ camera }
	, m_DoorEntered{ false }
{
	LoadFromFile(filePath);
	m_CurrentLevel = m_vLevels[0];
}

LevelManager::~LevelManager() noexcept
{
	for (int index{}; index < m_vLevels.size(); ++index)
	{
		delete m_vLevels[index];
		m_vLevels[index] = nullptr;
	}
	m_CurrentLevel = nullptr;
}

void LevelManager::Update(float elapsedSec)
{
	if (m_CurrentLevel != nullptr)
	{
		Door door;
		if (m_CurrentLevel->IsOnDoor(m_pKirby, door))
		{
			if (m_pKirby->HasEnteredDoor())
			{
				m_DoorEntered = true;
				ViewFade::StartFade(1.f);
			}

			if(m_DoorEntered)
			{
				if (ViewFade::IsFadingIn())
				{
					m_pKirby->SetPosition(door.outcomePos);
					m_DoorEntered = false;
					if (door.isFinalDoor)
					{
						m_CurrentLevel = m_vLevels[door.nextLevel];
						m_CurrentLevel->SetSubLevel(0);
						m_CurrentLevel->PlayMusic();
					}
					else
					{
						if (m_pKirby->GetPosition().y > door.doorRect.bottom)
						{
							m_pKirby->SetSavePoint(door.outcomePos);
							m_CurrentLevel->IncreaseSubLevel();
						}
						else m_CurrentLevel->DecreaseSubLevel();
					}
				}
			}
		}

		m_CurrentLevel->Update(elapsedSec, m_pKirby);
	}
}
void LevelManager::Draw() const
{
	if (m_CurrentLevel != nullptr)
	{
		m_CurrentLevel->Draw();
	}
}

void LevelManager::Reset()
{
	m_CurrentLevel = m_vLevels[0];
}

Level* LevelManager::GetCurrentLevel() const
{
	return m_CurrentLevel;
}

void LevelManager::LoadFromFile(const std::string& filePath)
{
	std::ifstream inputFile{ filePath };
	if (!inputFile)
	{
		std::cout << "Could not properly open the following file: " << filePath << "\n Please try again.\n";
		assert(inputFile && "could not open the file to load the level for the level manager class.");
	}
	else
	{
		std::stringstream sstream;
		std::string line;
		std::string element;
		while (std::getline(inputFile, line))
		{
			if (line != "\n" and line != "")
			{
				sstream << line << "\n";

				std::string tag;
				if (line[0] == '<')
				{
					tag = line.substr(1, line.find(">") - 1);
				}
				if (tag[0] == '/')
				{
					element = sstream.str();
					sstream.str("");

					AddLevel(element);
				}
			}
		}
	}
}
void LevelManager::AddLevel(const std::string& element)
{
	size_t startPos;
	size_t endPos;
	std::string subElement;

	std::vector<Door> doorsTemp;
	startPos = element.find("<DoorVector>");
	endPos = element.find("</DoorVector>") + 13;
	subElement = element.substr(startPos, endPos - startPos);
	AddDoors(subElement, doorsTemp);

	EnemyManager* enemyManagerTemp{ new EnemyManager(m_pCamera) };
	startPos = element.find("<EnemyManager>");
	endPos = element.find("</EnemyManager>") + 15;
	subElement = element.substr(startPos, endPos - startPos);
	AddEnemyManager(subElement, enemyManagerTemp);
	
	std::vector<PowerUp*> powerUpsTemp;
	startPos = element.find("<PowerUps>");
	endPos = element.find("</PowerUps>") + 11;
	subElement = element.substr(startPos, endPos - startPos);
	AddPowerUps(subElement, powerUpsTemp);

	std::vector<Animation> animsTemp;
	std::vector<Point2f> animsPosTemp;
	startPos = element.find("<Animations>");
	endPos = element.find("</Animations>") + 13;
	subElement = element.substr(startPos, endPos - startPos);
	AddAnimations(subElement, animsTemp, animsPosTemp);

	int subLevels{};
	subLevels = std::stoi(utils::GetAttributeValue("subLevels", element));

	std::string name;
	name = utils::GetAttributeValue("name", element);
	
	bool hasWater;
	hasWater = utils::ToBool(utils::GetAttributeValue("hasWater", element));

	Level* tempLevel = new Level(name, subLevels, enemyManagerTemp, doorsTemp, powerUpsTemp, hasWater, animsTemp, animsPosTemp);
	m_vLevels.push_back(std::move(tempLevel));

}
void LevelManager::AddEnemyManager(const std::string& element, EnemyManager* enemyManager)
{
	std::stringstream elementStringStream;
	elementStringStream << element;

	std::stringstream sstream;
	std::string line;
	std::string enemy;
	while (std::getline(elementStringStream, line))
	{
		if (line != "\n" and line != "" and line.find("EnemyManager") == std::string::npos)
		{
			sstream << line << "\n";

			std::string tag;
			size_t startPos{ line.find("<") };
			if (startPos != std::string::npos)
			{
				tag = line.substr(startPos + 1, line.find(">") - startPos - 1);
			}
			if (tag[0] == '/')
			{
				enemy = sstream.str();
				sstream.str("");

				Point2f spawnPoint = utils::ToPoint2f(utils::GetAttributeValue("spawnpoint", enemy));

				if (enemy.find("WaddleDee") != std::string::npos)
				{
					enemyManager->Add(new WaddleDee(spawnPoint));
				}
				if (enemy.find("WaddleDoo") != std::string::npos)
				{
					enemyManager->Add(new WaddleDoo(spawnPoint));
				}
				if (enemy.find("HotHead") != std::string::npos)
				{
					enemyManager->Add(new HotHead(spawnPoint));
				}
				if (enemy.find("Sparky") != std::string::npos)
				{
					enemyManager->Add(new Sparky(spawnPoint));
				}
				if (enemy.find("PoppyBrosJr") != std::string::npos)
				{
					enemyManager->Add(new PoppyBrosJr(spawnPoint));
				}
				if (enemy.find("BrontoBurt") != std::string::npos)
				{
					BrontoBurt::Tactic tactic;
					std::string tacticString = utils::GetAttributeValue("tactic", enemy);

					if (tacticString == "ascendingWave") tactic = BrontoBurt::Tactic::AscendingWave;
					else if (tacticString == "ascend") tactic = BrontoBurt::Tactic::Ascend;
					else if (tacticString == "straight") tactic = BrontoBurt::Tactic::Straight;
					else if (tacticString == "wave") tactic = BrontoBurt::Tactic::Wave;
					else tactic = BrontoBurt::Tactic::AscendingWave;

					enemyManager->Add(new BrontoBurt(spawnPoint, tactic));
				}
			}
		}
	}
}
void LevelManager::AddDoors(const std::string& element, std::vector<Door>& doors)
{
	std::stringstream elementStringStream;
	elementStringStream << element;

	std::stringstream sstream;
	std::string line;
	std::string doorString;
	while (std::getline(elementStringStream, line))
	{
		if (line != "\n" and line != "" and line.find("DoorVector") == std::string::npos)
		{
			sstream << line << "\n";

			std::string tag;
			size_t startPos{ line.find("<") };
			if (startPos != std::string::npos)
			{
				tag = line.substr(startPos + 1, line.find(">") - startPos - 1);
			}
			if (tag[0] == '/')
			{
				doorString = sstream.str();
				sstream.str("");


				Rectf doorRect		{ utils::ToRectf(utils::GetAttributeValue("door", doorString)) };
				Point2f doorOutcome	{ utils::ToPoint2f(utils::GetAttributeValue("outcomePos", doorString)) };
				bool isFinal		{ utils::ToBool(utils::GetAttributeValue("isFinal", doorString)) };
				int nextLevel		{ std::stoi(utils::GetAttributeValue("nextLevel", doorString)) };
				Door theDoor		{ doorRect, doorOutcome, isFinal, nextLevel };
				doors.push_back(theDoor);
			}
		}
	}
}
void LevelManager::AddPowerUps(const std::string& element, std::vector<PowerUp*>& powerUps)
{
	std::stringstream elementStringStream;
	elementStringStream << element;

	std::stringstream sstream;
	std::string line;
	std::string powerUpString;
	while (std::getline(elementStringStream, line))
	{
		if (line != "\n" and line != "" and line.find("PowerUps") == std::string::npos)
		{
			sstream << line << "\n";

			std::string tag;
			size_t startPos{ line.find("<") };
			if (startPos != std::string::npos)
			{
				tag = line.substr(startPos + 1, line.find(">") - startPos - 1);
			}
			if (tag[0] == '/')
			{
				powerUpString = sstream.str();
				sstream.str("");


				Point2f powerUpPos{ utils::ToPoint2f(utils::GetAttributeValue("position", powerUpString)) };

				if (powerUpString.find("Tomato") != std::string::npos)
				{
					powerUps.push_back(new PowerUp(powerUpPos, PowerUp::Type::Tomato));
				}
				if (powerUpString.find("Drink") != std::string::npos)
				{
					powerUps.push_back(new PowerUp(powerUpPos, PowerUp::Type::Drink));
				}
				if (powerUpString.find("Lollipop") != std::string::npos)
				{
					powerUps.push_back(new PowerUp(powerUpPos, PowerUp::Type::Lollipop));
				}
				if (powerUpString.find("LifeUp") != std::string::npos)
				{
					powerUps.push_back(new PowerUp(powerUpPos, PowerUp::Type::LifeUp));
				}
			}
		}
	}
}
void LevelManager::AddAnimations(const std::string& element, std::vector<Animation>& vAnims, std::vector<Point2f>& vAnimPos)
{
	std::stringstream elementStringStream;
	elementStringStream << element;

	std::stringstream sstream;
	std::string line;
	std::string animString;
	while (std::getline(elementStringStream, line))
	{
		if (line != "\n" and line != "" and line.find("Animations") == std::string::npos)
		{
			sstream << line << "\n";

			std::string tag;
			size_t startPos{ line.find("<") };
			if (startPos != std::string::npos)
			{
				tag = line.substr(startPos + 1, line.find(">") - startPos - 1);
			}
			if (tag[0] == '/')
			{
				animString = sstream.str();
				sstream.str("");

				Point2f animPos{ utils::ToPoint2f(utils::GetAttributeValue("pos", animString)) };
				Animation animNext{ std::vector<Frame>{}, true };

				if (animString.find("Water") != std::string::npos)
				{
					int offset{16};
					int width{ std::stoi(utils::GetAttributeValue("widthRep", animString)) };
					int height{ std::stoi(utils::GetAttributeValue("heightRep", animString)) };

					if (animString.find("WaterTop") != std::string::npos)
					{
						animNext = Animation{ std::vector<Frame>{
							Frame(Rectf(0, -78, 16, 16), 0.1f),
							Frame(Rectf(18, -78, 16, 16), 0.1f),
							Frame(Rectf(36, -78, 16, 16), 0.1f),
							Frame(Rectf(54, -78, 16, 16), 0.1f)
								}, true };
					}
					if (animString.find("WaterBody") != std::string::npos)
					{
						animNext = Animation{ std::vector<Frame>{
							Frame(Rectf(0, -60, 16, 16), 0.1f),
							Frame(Rectf(18, -60, 16, 16), 0.1f),
							Frame(Rectf(36, -60, 16, 16), 0.1f),
							Frame(Rectf(54, -60, 16, 16), 0.1f)
								}, true };
					}
					if (animString.find("WaterBodyHalf") != std::string::npos)
					{
						animNext = Animation{ std::vector<Frame>{
							Frame(Rectf(0, -68, 16, 8), 0.1f),
							Frame(Rectf(18, -68, 16, 8), 0.1f),
							Frame(Rectf(36, -68, 16, 8), 0.1f),
							Frame(Rectf(54, -68, 16, 8), 0.1f)
								}, true };
					}
					if (animString.find("WaterForeground") != std::string::npos)
					{
						animNext = Animation{ std::vector<Frame>{
							Frame(Rectf(0,  0, 16, 8), 0.1f),
							Frame(Rectf(18, 0, 16, 8), 0.1f),
							Frame(Rectf(36, 0, 16, 8), 0.1f),
							Frame(Rectf(54, 0, 16, 8), 0.1f)
								}, true };
					}
					if (animString.find("WaterBackgroundGrass") != std::string::npos)
					{
						offset = 8;
						animNext = Animation{ std::vector<Frame>{
							Frame(Rectf(0,  -182, 8, 8), 0.1f),
							Frame(Rectf(10, -182, 8, 8), 0.1f),
							Frame(Rectf(20, -182, 8, 8), 0.1f),
								}, true };
					}
					if (animString.find("WaterBackgroundWater") != std::string::npos)
					{
						offset = 8;
						animNext = Animation{ std::vector<Frame>{
							Frame(Rectf(0,  -172, 8, 8), 0.1f),
							Frame(Rectf(10, -172, 8, 8), 0.1f),
							Frame(Rectf(20, -172, 8, 8), 0.1f),
								}, true };
					}
					if (animString.find("WaterBackgroundSun") != std::string::npos)
					{
						offset = 8;
						animNext = Animation{ std::vector<Frame>{
							Frame(Rectf(0,  -192, 8, 8), 0.1f),
							Frame(Rectf(10, -192, 8, 8), 0.1f),
							Frame(Rectf(20, -192, 8, 8), 0.1f),
								}, true };
					}


					for (int widthIdx{}; widthIdx < width; ++widthIdx)
					{
						for (int heightIdx{}; heightIdx < height; ++heightIdx)
						{
							vAnimPos.push_back(Point2f(animPos.x + offset * widthIdx, animPos.y + offset * heightIdx));
							vAnims.push_back(animNext);
						}
					}

				}
				else if (animString.find("Cloud") != std::string::npos)
				{

					int left{ std::stoi(utils::GetAttributeValue("left", animString)) };
					int right{ std::stoi(utils::GetAttributeValue("right", animString)) };
					animPos.x -= 4;
					Point2f animNextPos = animPos;
					for (int index{}; index < left; ++index)
					{
						if (index % 2 == 0)
						{
							// cloudLeft outwards angle
							animNext = Animation{ std::vector<Frame>{
										Frame(Rectf(0,  -139, 8, 8), 0.1f),
										Frame(Rectf(10, -139, 8, 8), 0.1f),
										Frame(Rectf(0,  -139, 8, 8), 0.1f),
										Frame(Rectf(20, -139, 8, 8), 0.1f),
											}, true };
							animNextPos.x = animPos.x - (index / 2) * 8;
						}
						else
						{
							// cloudLeft inwards angle
							animNext = Animation{ std::vector<Frame>{
										Frame(Rectf(0,  -129, 8, 8), 0.1f),
										Frame(Rectf(10, -129, 8, 8), 0.1f),
										Frame(Rectf(0,  -129, 8, 8), 0.1f),
										Frame(Rectf(20, -129, 8, 8), 0.1f),
											}, true };
							animNextPos.y = animPos.y - ((index + 1) / 2) * 8;
						}

						vAnims.push_back(animNext);
						vAnimPos.push_back(animNextPos);
					}
					animPos.x += 8;
					animNextPos = animPos;
					for (int index{}; index < right; ++index)
					{
						if (index % 2 == 0)
						{
							// cloudRight outwards angle
							animNext = Animation{ std::vector<Frame>{
										Frame(Rectf(0,  -119, 8, 8), 0.1f),
										Frame(Rectf(10, -119, 8, 8), 0.1f),
										Frame(Rectf(0,  -119, 8, 8), 0.1f),
										Frame(Rectf(20, -119, 8, 8), 0.1f),
											}, true };
							animNextPos.x = animPos.x + (index / 2) * 8;
						}
						else
						{
							// cloudRight inwards angle
							animNext = Animation{ std::vector<Frame>{
										Frame(Rectf(0,  -109, 8, 8), 0.1f),
										Frame(Rectf(10, -109, 8, 8), 0.1f),
										Frame(Rectf(0,  -109, 8, 8), 0.1f),
										Frame(Rectf(20, -109, 8, 8), 0.1f),
											}, true };
							animNextPos.y = animPos.y -((index + 1) / 2) * 8;
						}

						vAnims.push_back(animNext);
						vAnimPos.push_back(animNextPos);
					}
				}


			}
		}
	}

}
