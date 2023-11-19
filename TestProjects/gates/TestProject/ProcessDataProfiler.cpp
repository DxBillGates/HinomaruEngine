#include "ProcessDataProfiler.h"
#include "Debug.h"

namespace
{
	// ���ϒl�̍X�V�Ԋu
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

		// fps�����l���傫���Ȃ肷���Ă��܂��\��������A���̏�ԂŌv�������float����𒴂��Ă��܂�����
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
		// �O�t���[���Ƀt�@�C���o�͂��Ă����Ȃ炱�̃t���[���ł̃f�[�^�X�V�͂��Ȃ��悤�ɂ���
		// ���@�t�@�C���o�͂ɂ�鏈�����ׂ��v�����Ă��܂�����
		if (m_IsOutputLastframe)
		{
			m_IsOutputLastframe = false;
			return;
		}

		// �f�[�^�擾
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
			// �O��̍X�V�����莞�Ԃ������畽�ϒl�̍X�V���s��
			if (elapsedTime > UPDATE_AVERAGE_TIMESPAN)
			{
				profileData->UpdateAverage(frameCount);

				// �X�V���̃g�[�^���l��ێ�
				m_PreElapsedTime = m_TotalElapsedTime;
				m_PreTotalFramecount = m_TotalFramecount;
			}

			// �g�[�^���̌o�ߎ��Ԃ���莞�Ԍo�����Ȃ�t�@�C���ɏo�͂���
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

		// �g�[�^���l�͖��t���X�V
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
		// �X�V�̍ۂɐV�����f�[�^���E���Ă����������疼�O�Ƃ͋t�̃f�[�^�ŏ���������
		m_Low = FLT_MAX;
		m_High = FLT_MIN;
	}

	void ProcessDataProfiler::Data::UpdateAverage(Int32 frameCount)
	{
		float data = m_Total - m_PreTotal;
		m_PreTotal = m_Total;
		m_Average = data / frameCount;

		// ���[�h���̃f�[�^���������Ă��܂��\�������邽�߈ꉞ����������
		InitMinMax();
	}

	void ProcessDataProfiler::Data::Update(float profileData)
	{
		// �ŏ��l�ő�l�͏�ɍX�V
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