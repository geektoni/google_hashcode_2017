#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <string>
#include <utility>
#include <cmath>

using namespace std;

struct cache {
	int size;
	vector<int> video;
};

struct ecache {
	int c;
	int latency;
};

struct request {
	int video_id;
	int nrequest;
};

struct endpoint {
	int d_latency;
	vector<ecache> ecaches;
	vector<request> requests;
	int total_requests;
};

vector<int> videos;
vector<cache> caches;
vector<endpoint> endpoints;

bool sort_cache(ecache & a, ecache & b) {
	return a.latency < b.latency;
}

bool sort_request(request & a, request & b) {
	return a.nrequest < b.nrequest;
}

bool sort_endpoints(endpoint & a, endpoint & b) {
	return a.total_requests < b.total_requests;
}

void resolve() {
	for (size_t e = 0; e < endpoints.size(); e++) {
		for (size_t r = 0; r < endpoints[e].requests.size(); r++) {
			for (size_t c = 0; c < endpoints[e].ecaches.size(); c++) {
				int cache_index = endpoints[e].ecaches[c].c;
				int video_index = endpoints[e].requests[r].video_id;
				if (caches[cache_index].size >= videos[video_index]) {
					auto it = find(caches[cache_index].video.begin(), caches[cache_index].video.end(), video_index);
					if (it == caches[cache_index].video.end()) {
						caches[cache_index].size -= videos[video_index];
						caches[cache_index].video.push_back(video_index);
						break;
					}
				}
			}
		}
	}
}

void solution() {
	int used = 0;
	for (size_t i = 0; i < caches.size(); i++) {
		if (caches[i].video.size() > 0) {
			used++;
		}
	}
	cout << used << endl;
	for (size_t i = 0; i < caches.size(); i++) {
		if (caches[i].video.size() > 0) {
			cout << i << " ";
			for (size_t j = 0; j < caches[i].video.size(); j++) {
				cout << caches[i].video[j] << " ";
			}
			cout << endl;
		}
	}

}

int V,E,R,C,X;

using namespace std;

int main(int argc, char ** argv) {
	cin >> V >> E >> R >> C >> X;

	// Caches initialization
	for (size_t i = 0; i < C; i++) {
		cache tmp;
		tmp.size = X;
		caches.push_back(tmp);
	}

	// Videos initialization
	videos = vector<int>(V);
	for (size_t i = 0; i < V; i++) {
		cin >> videos[i];
	}

	// Endpoint initialization
	for (size_t i = 0; i < E; i++) {
		int K;
		endpoint tmp;
		tmp.total_requests = 0;
		cin >> tmp.d_latency;
		cin >> K;
		for (size_t i = 0; i < K; i++) {
			ecache tmp2;
			cin >> tmp2.c;
			cin >> tmp2.latency;
			tmp.ecaches.push_back(tmp2);
		}
		sort(tmp.ecaches.begin(), tmp.ecaches.end(), sort_cache);
		endpoints.push_back(tmp);
	}

	// Request setup
	for (size_t i = 0; i < R; i++) {
		int RV, RE, RN;
		cin >> RV >> RE >> RN;
		request tmp;
		tmp.video_id = RV;
		tmp.nrequest = RN;
		endpoints[RE].requests.push_back(tmp);
		endpoints[RE].total_requests += RN;
	}

	// S endpoints
	sort(endpoints.begin(), endpoints.end(), sort_endpoints);

	// S request
	for (size_t i = 0; i < E; i++) {
		sort(endpoints[i].requests.begin(), endpoints[i].requests.end(), sort_request);
	}

	resolve();
	solution();

return 0;
}
