#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <bitset>
#include <algorithm>
#include <conio.h>
#include <Windows.h>

using namespace std;

struct GroupElement 
{
	vector<int> minterms; //숫자
	vector<int> value; //비트
};


pair<vector<int>, int> getMintermAndCount(const int minTerm); //반환하는 값은 4비트(벡터)와 
map<int, vector<GroupElement>> mergeGroupElement(map<int, vector<GroupElement>> numberedGroups, vector<GroupElement>& notUpdated);
void displayFormatedAnswer(set<vector<int>> answer);

// 출력함수
void printMinterm(const vector<int> v);
void printState(const vector<int> v);
void printGroups(const map<int, vector<GroupElement>> allGroups);
void printNotUpdated(vector<GroupElement> notUpdated);

int main()
{


	int arrow = 0;
	int key = 0;
	set<int> minTermsSet; //최소항 입력받을 트리 

	int mark[16];

	for (int i = 0; i < 16; i++)
		mark[i] = -1;


	auto print = [](int mark_[], int arrow_) {
		for (int i = 0; i < 16; i++)
		{
			if (arrow_ == i) cout << "▷";
			else cout << "  ";

			cout << bitset<4>(i) << '(' << i << ") : ";
			if (mark_[i] == 1) cout << "V";
			else cout << " ";

			cout << endl;
		}

	};

	while(true)
	{
		system("cls");
		print(mark, arrow);

		key = _getch();
		
		if (key == 13) break;
		else if (key == 32) mark[arrow] = mark[arrow]*-1;
		else if (key != 224) continue;
		else key = _getch();

		if (key == 72 && arrow != 0)--arrow;
		else if (key == 80 && arrow != 15) ++arrow;


	} 


	
	for (int i = 0; i < 16; i++)
	{
		if (mark[i] == 1)
			minTermsSet.insert(i);
	}
	


	map<int, vector<GroupElement>> numberedGroups; //그룹  


	for (set<int>::iterator it = minTermsSet.begin(); it != minTermsSet.end(); it++)
	{
		int term = *it;


		//pair자료형 result에 반환 되는 것은 first : 4비트(벡터) second : 1의 개수
		pair<vector<int>, int> result = getMintermAndCount(term);



		GroupElement groupEle;
		groupEle.minterms.push_back(term); //구조체에 항 삽입
		groupEle.value = result.first; // 구조체.value에 4비트(벡터) 대입

		//1의 개수 그룹에 구조체벡터(숫자, 4비트) 는 한 쌍
		numberedGroups[result.second].push_back(groupEle);


		// 민텀과 4비트 출력 
		cout << term << ": ";
		printMinterm(result.first);
		cout << endl;


	}

	// 모든 그룹 출력
	cout << "\n--------------------------------------------------\n";
	cout << "그룹으로 묶인 minterm : \n" << endl;
	printGroups(numberedGroups);

	// 그룹 머징 및 출력
	// notUpdated 는 선택 안된 그룹 


	vector<GroupElement> notUpdated;

	bool exit = true;

	while (exit)
	{
		//선택 못받은 민텀 noUpdated에 저장 및 머지된 그룹으로 초기화 
		map<int, vector<GroupElement>> newNumberedGroups = mergeGroupElement(numberedGroups, notUpdated);

		if (newNumberedGroups.size() == 0) //머지 다하면 나가기 {}.size == 0 
		{
			exit = false;
		}
		else
		{
			numberedGroups = newNumberedGroups;

			// 모든 그룹 출력
			cout << "\n--------------------------------------------------\n";
			cout << "minterm의 그룹 : \n" << endl;
			printGroups(numberedGroups);
		}
	}



	// 선택 안된거 출력 
	cout << "\n--------------------------------------------------\n";
	cout << "그룹 머징하고 남은거" << endl;
	printNotUpdated(notUpdated);

	// minterm과 테이블 

	// 테이블에 한번만 나타나

	map<int, int> countInTable;
	set<vector<int>> seenMinterms;

	//민텀 갯수 체크
	for (map<int, vector<GroupElement>>::iterator group = numberedGroups.begin(); group != numberedGroups.end(); group++)
	{
		for (vector<GroupElement>::iterator groupElement = group->second.begin(); groupElement != group->second.end(); groupElement++)
		{

			if (seenMinterms.count(groupElement->value))
				continue;
			seenMinterms.insert(groupElement->value);
	
			for (vector<int>::iterator minterm = groupElement->minterms.begin(); minterm != groupElement->minterms.end(); minterm++)
			{
				countInTable[*minterm]++;
			}
		}
	}

	//남겨진 민텀 개수 체크
	for (vector<GroupElement>::iterator groupElement = notUpdated.begin(); groupElement != notUpdated.end(); groupElement++)
	{

		if (seenMinterms.count(groupElement->value))
			continue;
		seenMinterms.insert(groupElement->value);
		// printState(groupElement->value);
		// cout << endl;
		for (vector<int>::iterator minterm = groupElement->minterms.begin(); minterm != groupElement->minterms.end(); minterm++)
		{
			countInTable[*minterm]++;
		}
	}


	set<vector<int>> answer;


	set<int> termsTaken;

	//countInTable에서 카르노로 그었을때 한번만해당하는 항 즉 epi에 해당하는 항을 찾음 
	//d.m 가 있는 minterm확인 밑 삽입

	for (map<int, int>::iterator cell = countInTable.begin(); cell != countInTable.end(); cell++)
	{
		if (cell->second == 1) //epi 만 추림
		{
			for (map<int, vector<GroupElement>>::iterator gp = numberedGroups.begin(); gp != numberedGroups.end(); gp++)
			{
				for (vector<GroupElement>::iterator ele = gp->second.begin(); ele != gp->second.end(); ele++)
				{
					int count = 0;
					for (vector<int>::iterator num = ele->minterms.begin(); num != ele->minterms.end(); num++)
					{
						if (cell->first == *num) //distinguish minterm이 있으면
							count++;
					}
					if (count != 0)
					{
						answer.insert(ele->value); //epi에 해당하는 4비트 넣기 

						//epi 텀 넣기
						for (vector<int>::iterator num = ele->minterms.begin(); num != ele->minterms.end(); ++num)
						{
							termsTaken.insert(*num); //epi해당하는 term 넣기 (set라 중복안됨)
						}
					}
				}
			}
		}
	}




	cout << "\n--------------------------------------------------\n";
	cout << "Terms with the occurnce of them: " << endl;
	for (map<int, int>::iterator i = countInTable.begin(); i != countInTable.end(); ++i)
	{
		cout << i->first << ": " << i->second << endl;
	}
	cout << endl << endl << endl;


	vector<int> termLeft; //빈공간
	vector <GroupElement> pi;
	map<int, vector<GroupElement>> numberedGroups_copy = numberedGroups;



 	for (map<int, int>::iterator i = countInTable.begin(); i != countInTable.end(); i++)
	{
		if (!termsTaken.count(i->first)) //epi에 해당하는 term에 없는 것만 추림 
		{
			termLeft.push_back(i->first);

			cout << "남은 항 : " << i->first << endl;
			for (map<int, vector<GroupElement>>::iterator gp = numberedGroups_copy.begin(); gp != numberedGroups_copy.end(); gp++)
			{
				for (vector<GroupElement>::iterator ele = gp->second.begin(); ele != gp->second.end(); )
				{
					bool flag = true;
 					for (vector<int>::iterator j = ele->minterms.begin(); j != ele->minterms.end()&&flag; j++)
					{
						
						if (i->first == *j)
						{
							cout << "pi중에서 : ";
							printState(ele->value);
							cout << endl;


							GroupElement temp = *ele;

							pi.push_back(temp);
							
							
							flag = false;
						}
						
					}

					if (flag)
						ele++;
					else
						ele = gp->second.erase(ele);
				}
			}



			// 남은거 있는지 확인
			for (vector<GroupElement>::iterator notUpdatedGroupElement = notUpdated.begin(); notUpdatedGroupElement != notUpdated.end(); )
			{
				bool flag = true;
				for (vector<int>::iterator minterm = notUpdatedGroupElement->minterms.begin(); minterm != notUpdatedGroupElement->minterms.end()&&flag; minterm++)
				{
					if (i->first == *minterm)
					{
						GroupElement temp = *notUpdatedGroupElement;
				
						pi.push_back(temp);
						cout << "남겨진 항 중에서";
						printState(notUpdatedGroupElement->value);
						cout << endl;

						flag = false;

					}
				}

				if (flag) 
					notUpdatedGroupElement++;
				else
					notUpdatedGroupElement = notUpdated.erase(notUpdatedGroupElement);

			}

		}
	}



	cout << "here" << endl;

	printNotUpdated(pi);


	cout << "left behind" << endl;
	for (size_t i = 0; i < termLeft.size(); i++)
	{
		cout << termLeft[i] << ' ';
	}

	cout << endl;
	
	

	while (termLeft.size()!=0 )
	{
		vector<int> maxCountedMinterm;

		GroupElement tmp;

		for (auto group = pi.begin(); group != pi.end(); group++)
		{
			
			vector<int> countedMinterms;

			vector<int>& mintermsOfGroup = group->minterms;
			for (auto minterm = mintermsOfGroup.begin(); minterm != mintermsOfGroup.end();minterm++)
			{
				
				if (	find(termLeft.begin(), termLeft.end(), *minterm) != termLeft.end()		)
				{
					countedMinterms.push_back(*minterm);
				}
			}

			if (maxCountedMinterm.size() < countedMinterms.size())
			{
				maxCountedMinterm = countedMinterms;
				tmp = *group;
			}
		}

		
		for (size_t i = 0; i < maxCountedMinterm.size(); i++)
		{
			termLeft.erase(		remove(	termLeft.begin(), termLeft.end(), maxCountedMinterm[i]	),	termLeft.end()				);
		}


		answer.insert(tmp.value);

	}

	
	printGroups(numberedGroups);


	cout << "\n--------------------------------------------------\n";
	cout << "간소화 식 : ";
	displayFormatedAnswer(answer);

	return 0;
}

//1의 개수와 4비트(벡터) 구하기
pair<vector<int>, int> getMintermAndCount(const int minTerm)
{
	vector<int> mintermVector;
	int onesCount = 0;

	for (int i = 0; i < 4; ++i)
	{
		if (minTerm & 1 << (3 - i )	)
		{
			mintermVector.push_back(1);
			onesCount++;
		}
		else
		{
			mintermVector.push_back(0);
		}
	}

	pair<vector<int>, int> mintermAndCount;
	mintermAndCount.first = mintermVector;
	mintermAndCount.second = onesCount;

	return mintermAndCount;
}

// 머지함수
map<int, vector<GroupElement>> mergeGroupElement(map<int, vector<GroupElement>> numberedGroups, vector<GroupElement>& notUpdated)
{
	// 머지된 그룹들은 여기 넣어짐
	map<int, vector<GroupElement>> newNumberedGroups;
	int updateCount = 0; //탈출용(마지막머지)

	set<vector<int>> termsUsed;

	for (map<int, vector<GroupElement>>::iterator group = numberedGroups.begin(); group != numberedGroups.end(); group++) //그룹 전체 반복
	{
		if (	numberedGroups.count(group->first + 1)	) //1의 개수 1차이 나는 항목이 있으면 
		{

			/*
				여기서부터 2중 for문
				groupElemtment에 있는 텀을 1이 더 많은 그룹인 nextGroup과 모두 비교함

			*/

			//group 이 가르치는건 pair(int, vector<GroupElement>>
			for (vector<GroupElement>::iterator groupElement = group->second.begin(); groupElement != group->second.end(); groupElement++)
			{

				//GroupElement가 가르치는건 구조체벡터(vector<GroupElement>)

				for (vector<GroupElement>::iterator nextGroupElement = numberedGroups[group->first + 1].begin(); nextGroupElement != numberedGroups[group->first + 1].end(); nextGroupElement++)
				{

					int diffIndex = -1;
					int diffCount = 0;

					for (int i = 0; i < 4; ++i)
					{
						if (groupElement->value[i] != nextGroupElement->value[i])
						{
							diffIndex = i; //다른 비트 위치 저장 
							diffCount++;
						}
					}

					if (diffCount == 1) //조건성립 1개수가 하나 차이남 
					{
						updateCount++;

						GroupElement newGroupElement;

						newGroupElement.value = groupElement->value; //반환할 GroupElement에 4비트 삽입
						newGroupElement.value[diffIndex] = -1; // 나중에 출력시 -로 조정

						// 0111 + 0011 = 0_11 꼴이된 민텀 생성

						newGroupElement.minterms = groupElement->minterms; //4비트의 정수값 삽입 


						for (vector<int>::iterator m = nextGroupElement->minterms.begin(); m != nextGroupElement->minterms.end(); m++)
						{
							newGroupElement.minterms.push_back(*m);
						}

						
						//비교한 민텀 구분 
						termsUsed.insert(groupElement->minterms);
						termsUsed.insert(nextGroupElement->minterms);

			
						newNumberedGroups[group->first].push_back(newGroupElement);
					}
				}
			}
		}
	}

	if (updateCount == 0) //머지할게 없으면
	{
		return {}; //size() == 0 용
	}

	for (map<int, vector<GroupElement>>::iterator group = numberedGroups.begin(); group != numberedGroups.end(); ++group)
	{
		for (vector<GroupElement>::iterator groupElement = group->second.begin(); groupElement != group->second.end(); ++groupElement)
		{
			//머지 안된 텀들 수집 

			if (!termsUsed.count(groupElement->minterms))
			{
				notUpdated.push_back(*groupElement);
			}
		}
	}

	return newNumberedGroups;
}

//최종 간소화식 출력
void displayFormatedAnswer(set<vector<int>> answer)
{
	for (set<vector<int>>::reverse_iterator i = answer.rbegin(); i != answer.rend(); ++i)
	{
		char letter = 'A';
		for (size_t j = 0; j < i->size(); ++j)
		{
			if ((*i)[j] != -1)
			{
				if ((*i)[j] == 1)
				{
					cout << letter;
				}
				else
				{
					cout << letter << '\'';
				}
			}
			letter++;
		}
		cout << " + ";
	}
	cout << "\b\b"
		<< "  " << endl;
}

//초기 출력
void printMinterm(const vector<int> v)
{
	for (size_t i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
}

//상태 출력
void printState(const vector<int> v)
{
	for (size_t i = 0; i < v.size(); ++i)
	{
		if (v[i] == -1)
		{
			cout << "_"
				<< " ";
		}
		else
		{
			cout << v[i] << " ";
		}
	}
}


// 그룹 별로 출력
void printGroups(map<int, vector<GroupElement>> allGroups)
{
	for (map<int, vector<GroupElement>>::iterator group = allGroups.begin(); group != allGroups.end(); ++group)
	{
		cout << "Group " << group->first << endl;

		for (vector<GroupElement>::iterator groupElement = group->second.begin(); groupElement != group->second.end(); ++groupElement)
		{
			printMinterm(groupElement->minterms);
			cout << ": ";
			printState(groupElement->value);
			cout << endl;
		}
	}
}

//선택 안된거 출력
void printNotUpdated(vector<GroupElement> notUpdated)
{
	cout << endl;
	for (vector<GroupElement>::iterator groupElement = notUpdated.begin(); groupElement != notUpdated.end(); ++groupElement)
	{
		printMinterm(groupElement->minterms);
		cout << ": ";
		printState(groupElement->value);
		cout << endl;
	}
	cout << endl;
}
