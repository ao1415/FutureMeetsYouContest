#include <iostream>

#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <cmath>
#include <stack>
#include <queue>

#include <cassert>

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
	/// <param name="_time">設定時間(マイクロ秒)</param>
	Timer(const std::chrono::microseconds& _time) { type = Type::microseconds; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(ミリ秒)</param>
	Timer(const std::chrono::milliseconds& _time) { type = Type::milliseconds; time = _time.count(); }

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

void show(const Array& arr) {

	for (const auto& a : arr)
	{
		assert(0 < a && a <= arr.size());
		cerr << a << ", ";
	}
	cerr << endl;
}

class AI {
private:

	/// <summary>
	/// 実行時間(ms)
	/// </summary>
	const int T = 4500;

	const double TempStart = 10000.0;
	const double TempEnd = 1.0;
	const double Time = T;
	const double TempDiff = (TempStart - TempEnd) / Time;

	XorShift random;
	int size;

	bool probability(const double& base, const double& next, const long long& t) {

		const double diff = base - next;

		if (diff > 0) return true;

		const double temp = TempStart - TempDiff * t;

		const double p = exp(diff / temp) * 4294967295.0;

		return p > random.rand();
	}

	int getMaxAdd(const Array& field, const int idx1, const int idx2, const int width) {

		int minV = size;
		int maxV = 0;

		for (int i = idx1; i < idx1 + width; i++)
		{
			minV = min(minV, field[i]);
		}
		for (int i = idx2; i < idx2 + width; i++)
		{
			maxV = max(maxV, field[i]);
		}

		return min(minV - 1, size - maxV);
	}

	int getScore(const Array& a, const Answer& ans) {

		int score = 0;

		for (int s = ans.i; s <= ans.j; s++)
		{
			score += abs(s + 1 - (a[s] - ans.v));
		}

		for (int s = ans.k; s <= ans.l; s++)
		{
			score += abs(s + 1 - (a[s] + ans.v));
		}

		return score;
	}
	int getScore(const Array& a) {

		int score = 0;

		for (int i = 0, size = (int)a.size(); i < size; i++)
		{
			score += abs(i + 1 - a[i]);
		}

		return score;
	}

public:

	/// <summary>
	/// Thinks the specified arr.
	/// </summary>
	/// <param name="arr">The arr.</param>
	/// <param name="K">操作回数</param>
	/// <returns></returns>
	vector<Answer> think(const Array& arr, const int K) {

		size = (int)arr.size();

		vector<Answer> ans(K);

		Array field = arr;

		vector<pair<int, int>> sf(size);
		for (int i = 0; i < size; i++)
		{
			sf[i] = { arr[i],i };
		}

		sort(sf.begin(), sf.end());

		for (int t = 0; t < 100; t++)
		{
			ans[t] = Answer{ sf[size - t - 1].second, sf[size - t - 1].second, sf[t].second, sf[t].second, size / 2 };

			simulator(field, ans[t]);
		}

		for (int t = 100; t < K; t++)
		{
			Timer timer(chrono::microseconds(4900 * 1000 / K));

			pair<long long int, Answer> best{ 9223372036854775807,Answer{0,0,1,1,0} };

			timer.start();

			const int width = (int)max(1.0, (size / 8) * (1 - (double)t / K));
			while (!timer)
			{

				const int p1 = random.rand() % (size - width - width);
				const int p2 = p1 + width + random.rand() % (size - (p1 + width) - width);

				//if (p1 != p2)
				{
					const int idx1 = min(p1, p2);
					const int idx2 = max(p1, p2);

					const int MaxAdd = getMaxAdd(field, idx1, idx2, width);

					if (MaxAdd > 0)
					{
						const int add = random.rand() % (MaxAdd + 1);

						const Answer com{ idx1,idx1 + width - 1,idx2,idx2 + width - 1,add };

						long long int score = getScore(field, com);

						//cerr << width << ", " << add << ", " << score << endl;

						if (score < best.first)
						{
							best.first = score;
							best.second = com;
						}
					}
				}
			}

			ans[t] = best.second;
			simulator(field, best.second);

		}

		return ans;
	}

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
	/*
	for (const auto& a : ans)
	{
		simulator(A, a);
		show(A);
	}
	*/
	for (const auto& a : ans)
	{
		cout << a.i + 1 << " " << a.j + 1 << " " << a.k + 1 << " " << a.l + 1 << " " << a.v << endl;
	}

	return 0;
}
