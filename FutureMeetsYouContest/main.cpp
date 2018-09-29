#include <iostream>

#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <cmath>
#include <stack>
#include <queue>

using namespace std;

using Array = vector<int>;

/// <summary>
/// 一定時間の経過したか確認するクラス
/// </summary>
class Timer {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Timer() = default;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(ナノ秒)</param>
	Timer(const std::chrono::nanoseconds& _time) { type = Type::nanoseconds; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(マイクロ秒)</param>
	Timer(const std::chrono::microseconds& _time) { type = Type::microseconds; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(ミリ秒)</param>
	Timer(const std::chrono::milliseconds& _time) { type = Type::milliseconds; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(秒)</param>
	Timer(const std::chrono::seconds& _time) { type = Type::seconds; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(分)</param>
	Timer(const std::chrono::minutes& _time) { type = Type::minutes; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(時)</param>
	Timer(const std::chrono::hours& _time) { type = Type::hours; time = _time.count(); }

	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(ナノ秒)</param>
	void set(const std::chrono::nanoseconds& _time) { type = Type::nanoseconds; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(マイクロ秒)</param>
	void set(const std::chrono::microseconds& _time) { type = Type::microseconds; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(ミリ秒)</param>
	void set(const std::chrono::milliseconds& _time) { type = Type::milliseconds; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(秒)</param>
	void set(const std::chrono::seconds& _time) { type = Type::seconds; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(分</param>
	void set(const std::chrono::minutes& _time) { type = Type::minutes; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(時)</param>
	void set(const std::chrono::hours& _time) { type = Type::hours; time = _time.count(); }

	/// <summary>
	/// タイマーを開始させる
	/// </summary>
	void start() { s = std::chrono::high_resolution_clock::now(); }

	inline long long diff() const {
		const auto e = std::chrono::high_resolution_clock::now();
		switch (type)
		{
		case Type::nanoseconds: return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count();
		case Type::microseconds: return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count();
		case Type::milliseconds: return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count();
		case Type::seconds: return std::chrono::duration_cast<std::chrono::seconds>(e - s).count();
		case Type::minutes: return std::chrono::duration_cast<std::chrono::minutes>(e - s).count();
		case Type::hours: return std::chrono::duration_cast<std::chrono::hours>(e - s).count();
		default: return true;
		}
	}

	/// <summary>
	/// 設定時間経過したかを得る
	/// </summary>
	/// <returns>経過していれば true, それ以外は false</returns>
	inline const bool check() const {
		return diff() >= time;
	}

	/// <summary>
	/// 設定時間経過したかを得る
	/// </summary>
	/// <returns>経過していれば true, それ以外は false</returns>
	operator bool() const { return check(); }

private:

	enum class Type {
		nanoseconds,
		microseconds,
		milliseconds,
		seconds,
		minutes,
		hours
	};

	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	long long time;
	Type type;

};

struct XorShift {
	unsigned int x;
	XorShift() : x(2463534242U) {}
	unsigned int rand() {
		x ^= (x << 13);
		x ^= (x >> 17);
		x ^= (x << 5);
		return x;
	}

};

const double inline random() {

	static random_device rnd;
	static mt19937 mt(rnd());

	static uniform_real_distribution<> bet(0, 1.0);

	return bet(mt);
}

int GetInteger() {
	int n;
	cin >> n;
	return n;
}

struct Answer {
	int i = 0;
	int j = 1;
	int k = 2;
	int l = 3;
	int v = 0;
};

void simulator(Array& a, const Answer& ans) {

	for (int s = ans.i; s <= ans.j; s++)
	{
		a[s] -= ans.v;
	}

	for (int s = ans.k; s <= ans.l; s++)
	{
		a[s] += ans.v;
	}
};

int getScore(const Array& a, const Answer& ans) {

	int score = 0;

	for (int s = ans.i; s <= ans.j; s++)
	{
		score += abs(s - a[s]) - abs(s - a[s] - ans.v);
	}

	for (int s = ans.k; s <= ans.l; s++)
	{
		score += abs(s - a[s]) - abs(s - a[s] + ans.v);
	}

	return score;
}

void show(const Array& arr) {

	for (const auto& a : arr)
	{
		cerr << a << ", ";
	}
	cerr << endl;
}

class AI {
public:

	/// <summary>
	/// Thinks the specified arr.
	/// </summary>
	/// <param name="arr">The arr.</param>
	/// <param name="K">操作回数</param>
	/// <returns></returns>
	vector<Answer> think(const Array& arr, const int K) {

		const int size = (int)arr.size();

		vector<Answer> ans(K);

		queue<pair<int, int>> sta;

		sta.push({ 0,size - 1 });

		auto a = arr;

		for (int i = 0; i < K; i++)
		{
			if (!sta.empty())
			{
				const auto section = sta.front();
				sta.pop();

				const int width = section.second - section.first;
				const int sec1s = section.first;
				const int sec1e = sec1s + width / 2;
				const int sec2s = sec1e + 1;
				const int sec2e = section.second;

				int minValue = size;
				for (int s = sec1s; s <= sec1e; s++)
				{
					minValue = min(minValue, a[s]);
				}
				int maxValue = 0;
				for (int s = sec2s; s <= sec2e; s++)
				{
					maxValue = max(maxValue, a[s]);
				}

				int add = min(minValue - 0, size - maxValue);

				int maxV = 0;
				int maxScore = 0;
				for (int v = 1; v <= add; v++)
				{
					Answer com{ sec1s,sec1e,sec2s,sec2e,v };

					const int score = getScore(a, com);

					if (score > maxScore)
					{
						maxScore = score;
						maxV = v;
					}
				}

				auto next = a;
				const auto com = Answer{ sec1s,sec1e,sec2s,sec2e,maxV };
				simulator(next, com);

				a = next;
				ans[i] = com;

				sta.push({ sec1s,sec1e });
				sta.push({ sec2s,sec2e });
			}
		}

		return ans;
	}

private:



};

int main() {

	const int N = GetInteger();
	const int K = GetInteger();

	vector<int> A(N);

	for (auto& v : A)
	{
		cin >> v;
	}

	AI ai;

	const auto& ans = ai.think(A, K);

	for (const auto& a : ans)
	{
		cout << a.i + 1 << " " << a.j + 1 << " " << a.k + 1 << " " << a.l + 1 << " " << a.v << endl;
	}

	return 0;
}
