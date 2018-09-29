#include <iostream>

#include <vector>
#include <string>

using namespace std;

int GetInteger() {
	int n;
	cin >> n;
	return n;
}

struct Answer {
	int i = 0;
	int j = 0;
	int k = 1;
	int l = 1;
	int v = 0;
};

int main() {

	const int N = GetInteger();
	const int K = GetInteger();

	vector<int> A(N);
	vector<Answer> ans(K);

	for (auto& v : A)
	{
		cin >> v;
	}

	for (const auto& a : ans)
	{
		cout << a.i << " " << a.j << " " << a.k << " " << a.l << " " << a.v << endl;
	}

	return 0;
}
