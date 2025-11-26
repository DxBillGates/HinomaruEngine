struct Point3
{
	float x, y, z;
};

struct Quat
{
	float x, y, z, w;
};

struct Vector3
{
	float x, y, z;
};

namespace btl
{
namespace tools
{
namespace formation
{
	class Editor
	{
	private:
		/*
		Flags flags; 
		Formation fomation;
		widget::Manager widgetManager;
		unit::Manager unitManager;
		gizmo::Manager gizmoManager;
		undoredo::Manager undoredoManager;
		input::Keyboard keyInput;
		*/
	public:
		void Setup()
		{
			/*
			
			*/
		}

		void Run()
		{
			int categoryIndex = 0;
			int prevCategoryIndex = categoryIndex;

			// タスク側
			auto TaskUpdate = []()
			{
				/*
				// 何かしらロード中、セーブ中ならタスク側の更新はしないようにする
				if(flags.Is(LOADING | SAVING))return;

				keyInput.Update();

				widgetManager.Update();
				unitManager.Update();
				gizmoManager.Update();

				undoredoManager.Update();
				*/
			};

			auto TaskRender = []()
			{
				/*
				unitManager.Render();
				*/
			};
			

			// ファイバー側
			bool isExit = false;
			while (!isExit)
			{
				/*
				// カテゴリが切り替わった際の処理
				if(prevCategoryIndex != categoryIndex)
				{
					flags.Set(LOADING);
					// btlActionの切り替えやフォーメーションのデータセーブ
					unitManager.FiberSecuence();
				}

				prevCategoryIndex = categoryIndex;
				*/
			}
		}

		void Terminate()
		{

		}
	};
}
}
}

int main()
{
	namespace btlTools = btl::tools;
	btlTools::formation::Editor editor;
	editor.Setup();
	editor.Run();
	editor.Terminate();
}

namespace btl
{
namespace tools
{
namespace formation
{
	namespace unit
	{
		struct EditorUnit
		{
			struct Rotate
			{
				Quat quat;
				Vector3 euler;
				Vector3 ypr;
			};
			Point3 pos;
			Rotate rotate;
			Vector3 scale;
		};

		class Unit
		{
		private:
			EditorUnit unitData;
		};

		class ICameraUnit : public Unit
		{
		};

		class CameraUnit : public ICameraUnit
		{
		public:
			virtual void Update();
			virtual void Render();
		};

		class SceneCameraUnit : public CameraUnit
		{

		};

		class ICharaUnit : public Unit
		{
		};

		class CharaUnit : public ICharaUnit
		{
		public:
			virtual void Update();
			virtual void Render();
		};

		class PlayerUnit : public CharaUnit
		{

		};

		class EnemyUnit : public CharaUnit
		{
		};

#include <string>
#include <map>
#include <vector>
		class Manager
		{
		private:
			std::map<std::string, std::vector<Unit*>> units;
			std::vector<ICharaUnit*> enablePlayerUnits;
			std::vector<ICharaUnit*> enableEnemyUnits;
			ICameraUnit* pSceneCamera;
		public:
			void Setup()
			{
				units.insert(std::make_pair<std::string, std::vector<Unit*>>("Camera", {}));
				units.insert(std::make_pair<std::string, std::vector<Unit*>>("Player", {}));
				units.insert(std::make_pair<std::string, std::vector<Unit*>>("Enemy", {}));
			}

			void Update()
			{
				((CameraUnit*)pSceneCamera)->Update();

				for (auto& pPlayerUnit : enablePlayerUnits)
				{
					CharaUnit* pChara = (CharaUnit*)pPlayerUnit;
					pChara->Update();
				}

				for (auto& pEnemyUnit : enableEnemyUnits)
				{
					CharaUnit* pChara = (CharaUnit*)pEnemyUnit;
					pChara->Update();
				}

				// 敵グループ全体の回転補正

				// 敵の体の向きをカメラ側に補正
			}

			void FiberSecuence()
			{
				// フォーメーションデータに保存

				// カテゴリ切り替えに伴うアクションの切り替え

				// 有効ユニットの切り替え

				// ユニットに持たせておくフォーメーションデータの切り替え
			}

			void Render()
			{
				((CameraUnit*)pSceneCamera)->Render();
			}
		};
	}
}
}
}