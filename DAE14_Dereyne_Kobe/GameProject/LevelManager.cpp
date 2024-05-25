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
	m_CurrentLevel->PlayMusic();
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
						if (m_pKirby->GetPosition().y > door.doorRect.bottom) m_CurrentLevel->IncreaseSubLevel();
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

	int subLevels{};
	subLevels = std::stoi(utils::GetAttributeValue("subLevels", element));

	std::string name;
	name = utils::GetAttributeValue("name", element);

	//TODO check if I can/should make lvls non-ptrs, same for NeemyMngr in level class
	Level* tempLevel = new Level(name, subLevels, enemyManagerTemp, doorsTemp);
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
			size_t frameStartPos{ line.find("<") };
			if (frameStartPos != std::string::npos)
			{
				tag = line.substr(frameStartPos + 1, line.find(">") - frameStartPos - 1);
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
