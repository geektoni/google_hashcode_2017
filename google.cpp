#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <string>
#include <utility>
#include <cmath>

using namespace std;

int V,E,R,C,X;

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
	vector<bool> videos_inserted;
	int total_requests;
	int total_latency;
};

vector<int> videos;
vector<cache> caches;
vector<endpoint> endpoints;
vector<vector<int>> videos_end;

bool sort_cache(ecache & a, ecache & b) {
	return a.latency < b.latency;
}

bool sort_request(request & a, request & b) {
	return (a.nrequest/(double)videos[a.video_id]) > (b.nrequest/(double)videos[b.video_id]);
	//return a.nrequest > b.nrequest;
}

bool sort_endpoints(endpoint & a, endpoint & b) {
	return (a.total_requests*a.d_latency) > (b.total_requests*b.d_latency);
}

int find_video(int j, vector<ecache> v) {
	for (size_t i = 0; i < v.size(); i++) {
		auto it = find(caches[v[i].c].video.begin(), caches[v[i].c].video.end(), j);
		if (caches[v[i].c].video.end() != it) {
			return v[i].c;
		}
	}
	return -1;
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
						int where_is = find_video(video_index, endpoints[e].ecaches);
						if (where_is != -1) {
							if ((double)endpoints[e].ecaches[c].latency < endpoints[e].ecaches[where_is].latency) {
								caches[cache_index].size -= videos[video_index];
								caches[cache_index].video.push_back(video_index);
								break;
							}
						} else {
							caches[cache_index].size -= videos[video_index];
							caches[cache_index].video.push_back(video_index);
							break;
						}
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

int count_end(int j, vector<int> & v) {
	int counter=0;
	for (size_t i = 0; i < v.size(); i++) {
		for (size_t k = 0; k < endpoints[v[i]].ecaches.size(); k++) {
			if (endpoints[v[i]].ecaches[k].c == j){
				counter++;
			}
		}
	}
	return counter;
}

void allocate() {
	for (size_t i = 0; i < videos_end.size(); i++) {
		int c=-1; int n_endpoint_max=-1;
		for (size_t j = 0; j < E; j++) {
			int c_temp = j;
			int n_endpoint = count_end(j, videos_end[i]);
			if (n_endpoint > n_endpoint_max) {
				c = c_temp;
				n_endpoint_max = n_endpoint;
			}
		}
		if (c != -1 && n_endpoint_max != -1 && n_endpoint_max != 1) {
			if (caches[c].size >= videos[i]) {
				caches[c].size -= videos[i];
				caches[c].video.push_back(i);
				for (size_t k = 0; k < videos_end[i].size(); k++) {
					// Take endpoint index
					int endpoint_index = videos_end[i][k];
					// For all endpoint requests, find the one with the video_id
					// that we have inserted. And set it to true
					for (size_t p = 0; p < endpoints[endpoint_index].requests.size(); p++) {
						if(endpoints[endpoint_index].requests[p].video_id == i) {
							endpoints[endpoint_index].videos_inserted[p] = true;
						}
					}
				}
			}
		}
	}
}

int main(int argc, char ** argv) {
	cin >> V >> E >> R >> C >> X;

	// Caches initialization
	for (size_t i = 0; i < C; i++) {
		cache tmp;
		tmp.size = X;
		caches.push_back(tmp);
	}

	// Videos initialization
	videos_end = vector<vector<int>>(V);
	videos = vector<int>(V);
	for (size_t i = 0; i < V; i++) {
		cin >> videos[i];
	}

	// Endpoint initialization
	for (size_t i = 0; i < E; i++) {
		int K;
		endpoint tmp;
		tmp.total_requests = 0;
		tmp.total_latency = 0;
		tmp.videos_inserted = vector<bool>(V, false);
		cin >> tmp.d_latency;
		cin >> K;
		for (size_t i = 0; i < K; i++) {
			ecache tmp2;
			cin >> tmp2.c;
			cin >> tmp2.latency;
			tmp.total_latency += tmp2.latency;
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
		videos_end[RV].push_back(RE);
		endpoints[RE].requests.push_back(tmp);
		endpoints[RE].total_requests += RN;
	}

	// S endpoints
	sort(endpoints.begin(), endpoints.end(), sort_endpoints);

	// S request
	for (size_t i = 0; i < E; i++) {
		sort(endpoints[i].requests.begin(), endpoints[i].requests.end(), sort_request);
	}

	allocate();
	resolve();
	solution();

return 0;
}
