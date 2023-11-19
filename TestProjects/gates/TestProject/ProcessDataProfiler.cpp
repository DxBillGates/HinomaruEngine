#include "ProcessDataProfiler.h"
#include "Debug.h"

namespace
{
	// 平均値の更新間隔
	static const float UPDATE_AVERAGE_TIMESPAN = 5.f;
	static const std::vector<std::string> PROFILE_DATA_NAMES =
	{
		"FPS",
		"CPU",
		"GPU",
		"OBJECTS",
		"TRIANGLES",
		"VERTICES",
	};

}

namespace he
{
	class ProcessDataProfiler
	{
	private:
		struct Data
		{
			std::string m_Name;
			float m_Low;
			float m_High;
			float m_Average;
			float m_PreTotal;
			float m_Total;

			Data()
				: m_Low(0), m_High(0), m_PreTotal(0), m_Total(0), m_Average(0)
			{

			}

			void Initialize();
			void InitMinMax();
			void UpdateAverage(Int32 frameCount);
			void Update(float profileData);
			virtual void Render(float x, float y);

			const float GetTotal() const;
		};

		// fpsだけ値が大きくなりすぎてしまう可能性があり、その状態で計測するとfloat上限を超えてしまうため
		struct FpsData : public Data
		{
		public:
			void Render(float x, float y) override;
		};
	private:
		float m_PreElapsedTime;
		float m_TotalElapsedTime;
		Int32 m_PreTotalFramecount;
		Int32 m_TotalFramecount;
		Bool m_IsOutputLastframe;
		std::vector<Data*> profileDatas;
	private:
		void OutputDatas();
	public:
		ProcessDataProfiler();
		~ProcessDataProfiler();
		void Initialize();
		void Update(float deltaTime = DELTATIME60, const float PROFILING_TIME = 30);
	};

	static ProcessDataProfiler profiler;

	// -------- ProcessDataProfiler ---------------------------------------------------------------- //

	void ProcessDataProfiler::OutputDatas()
	{
	}

	ProcessDataProfiler::ProcessDataProfiler()
		: m_PreElapsedTime(0)
		, m_TotalElapsedTime(0)
		, m_PreTotalFramecount(0)
		, m_TotalFramecount(0)
		, m_IsOutputLastframe(false)
	{
		profileDatas.push_back(new FpsData());
		auto& pFpsData = profileDatas.back();
		pFpsData->m_Name = PROFILE_DATA_NAMES[0];

		for (Int32 i = 1; i < PROFILE_DATA_NAMES.size(); ++i)
		{
			profileDatas.push_back(new Data());
			auto& profileData = profileDatas.back();
			profileData->m_Name = PROFILE_DATA_NAMES[i];
		}
	}

	ProcessDataProfiler::~ProcessDataProfiler()
	{
		for (auto& pProfileData : profileDatas)
		{
			delete pProfileData;
		}
	}

	void ProcessDataProfiler::Initialize()
	{
		for (auto& profileData : profileDatas)
		{
			profileData->Initialize();
		}
	}

	void ProcessDataProfiler::Update(float deltaTime, const float PROFILING_TIME)
	{
		// 前フレームにファイル出力していたならこのフレームでのデータ更新はしないようにする
		// ＊　ファイル出力による処理負荷を計測してしまうため
		if (m_IsOutputLastframe)
		{
			m_IsOutputLastframe = false;
			return;
		}

		// データ取得
		float fpsData = deltaTime;
		float cpuData = 0;
		float gpuData = 0;
		float objects = 0;
		float triangles = 0;
		float vertices = 0;

		const float* datas[] = { &fpsData,&cpuData,&gpuData,&objects,&triangles,&vertices };

		float elapsedTime = m_TotalElapsedTime - m_PreElapsedTime;
		Int32 frameCount = m_TotalFramecount - m_PreTotalFramecount;

		Int32 count = 0;
		for (auto& profileData : profileDatas)
		{
			// 前回の更新から一定時間たったら平均値の更新を行う
			if (elapsedTime > UPDATE_AVERAGE_TIMESPAN)
			{
				profileData->UpdateAverage(frameCount);

				// 更新時のトータル値を保持
				m_PreElapsedTime = m_TotalElapsedTime;
				m_PreTotalFramecount = m_TotalFramecount;
			}

			// トータルの経過時間が一定時間経ったならファイルに出力する
			if (m_TotalElapsedTime > PROFILING_TIME)
			{
				m_PreTotalFramecount = m_TotalFramecount = 0;
				m_IsOutputLastframe = true;
			}


			profileData->Update(*datas[count++]);

			if (profileData->m_Name.compare("FPS") == 0)
			{
				profileData->Render(0, 0);
			}
		}

		// トータル値は毎フレ更新
		m_TotalElapsedTime += deltaTime;
		++m_TotalFramecount;
	}

	// -------- Data ---------------------------------------------------------------- //

	void ProcessDataProfiler::Data::Initialize()
	{
		InitMinMax();

		m_Average = m_Total = 0;
	}

	void ProcessDataProfiler::Data::InitMinMax()
	{
		// 更新の際に新しいデータを拾っていきたいから名前とは逆のデータで初期化する
		m_Low = FLT_MAX;
		m_High = FLT_MIN;
	}

	void ProcessDataProfiler::Data::UpdateAverage(Int32 frameCount)
	{
		float data = m_Total - m_PreTotal;
		m_PreTotal = m_Total;
		m_Average = data / frameCount;

		// ロード中のデータが混じってしまう可能性があるため一応初期化する
		InitMinMax();
	}

	void ProcessDataProfiler::Data::Update(float profileData)
	{
		// 最小値最大値は常に更新
		if (profileData > m_High)m_High = profileData;
		if (profileData < m_Low)m_Low = profileData;

		m_Total += profileData;
	}

	void ProcessDataProfiler::Data::Render(float x, float y)
	{
		// ave, high, low
		PrintLog("ave : %f, high : %f, low : %f\n", m_Average, m_High, m_Low);
	}

	const float ProcessDataProfiler::Data::GetTotal() const
	{
		return m_Total;
	}

	void ProcessDataProfiler::FpsData::Render(float x, float y)
	{
		// ave, high, low
		PrintLog("ave : %f, high : %f, low : %f\n",m_Average == 0 ? 0 : 1.f / m_Average, 1.f / m_Low, 1.f / m_High);
	}

	void InitializeProfileData()
	{
		profiler.Initialize();
	}

	void UpdateProfileData(float deltaTime, const float PROFILING_TIME)
	{
		profiler.Update(deltaTime, PROFILING_TIME);
	}
}