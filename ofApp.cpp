#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->frame_span = 30;
	this->size = 15;
	while (this->location_list.size() < 120) {

		auto location = glm::vec2(ofRandom(80, ofGetWidth() - 80), ofRandom(80, ofGetHeight() - 80));
		bool flag = true;
		for (int i = 0; i < this->location_list.size(); i++) {

			if (glm::distance(location, this->location_list[i]) < this->size * 3) {

				flag = false;
				break;
			}
		}

		if (flag) {

			this->location_list.push_back(location);
		}
	}

	ofColor color;
	vector<int> hex_list = { 0xf44336, 0x9C27B0, 0x3F51B5, 0x03A9F4, 0x009688, 0x8BC34A, 0xFFEB3B, 0xFF9800 };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}

	for (int i = 0; i < 90; i++) {

		this->current_index_list.push_back(0);
		this->next_index_list.push_back(ofRandom(this->location_list.size()));
		this->color_list.push_back(this->base_color_list[ofRandom(this->base_color_list.size())]);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < this->current_index_list.size(); i++) {

		if (ofGetFrameNum() % (this->frame_span * 2) == 0) {

			int next_index = this->next_index_list[i];
			int current_index = next_index;

			vector<int> near_indexes;
			for (int location_index = 0; location_index < this->location_list.size(); location_index++) {

				if (current_index == location_index) { continue; }
				if (glm::distance(this->location_list[current_index], this->location_list[location_index]) < this->size * 5) {

					near_indexes.push_back(location_index);
				}
			}

			if (near_indexes.size() > 0) {

				next_index = near_indexes[ofRandom(near_indexes.size())];
			}

			this->current_index_list[i] = current_index;
			this->next_index_list[i] = next_index;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	int frame_param = ofGetFrameNum() % this->frame_span;

	for (int i = 0; i < this->next_index_list.size(); i++) {

		ofSetColor(this->color_list[i]);

		int current_index = this->current_index_list[i];
		int next_index = this->next_index_list[i];

		auto angle_current = std::atan2(this->location_list[next_index].y - this->location_list[current_index].y, this->location_list[next_index].x - this->location_list[current_index].x);
		auto satellite_point_current = this->location_list[current_index] + glm::vec2(this->size * cos(angle_current), this->size * sin(angle_current));
		auto angle_next = std::atan2(this->location_list[current_index].y - this->location_list[next_index].y, this->location_list[current_index].x - this->location_list[next_index].x);
		auto satellite_point_next = this->location_list[next_index] + glm::vec2(this->size * cos(angle_next), this->size * sin(angle_next));

		if (ofGetFrameNum() % (this->frame_span * 2) < this->frame_span) {

			ofSetLineWidth(3);
			auto distance = glm::distance(satellite_point_next, satellite_point_current);
			distance = ofMap(frame_param, 0, this->frame_span, 0, distance);
			auto direction = satellite_point_next - satellite_point_current;
			ofDrawLine(satellite_point_current, satellite_point_current + glm::normalize(direction) * distance);

			ofSetLineWidth(1.5);
			for (int location_index = 0; location_index < this->location_list.size(); location_index++) {

				ofNoFill();
				ofSetColor(255);
				ofDrawCircle(this->location_list[location_index], this->size);

				if (current_index == location_index) {

					int deg_param = ofMap(frame_param, 0, this->frame_span, 180, 0);
					float start_deg = angle_current * RAD_TO_DEG;

					ofFill();
					ofSetColor(this->color_list[i]);
					ofBeginShape();
					for (int deg = start_deg; deg <= start_deg + deg_param; deg++) {

						ofVertex(this->location_list[location_index] + glm::vec2(this->size * cos(deg * DEG_TO_RAD), this->size * sin(deg * DEG_TO_RAD)));
					}
					for (int deg = start_deg - deg_param; deg <= start_deg; deg++) {

						ofVertex(this->location_list[location_index] + glm::vec2(this->size * cos(deg * DEG_TO_RAD), this->size * sin(deg * DEG_TO_RAD)));
					}

					ofEndShape();
				}

				if (next_index == location_index) {

					int deg_param = ofMap(frame_param, 0, this->frame_span, 0, 90);
					float start_deg = angle_next * RAD_TO_DEG;

					ofFill();
					ofSetColor(this->color_list[i]);
					ofBeginShape();
					for (int deg = start_deg; deg <= start_deg + deg_param; deg++) {

						ofVertex(this->location_list[location_index] + glm::vec2(this->size * cos(deg * DEG_TO_RAD), this->size * sin(deg * DEG_TO_RAD)));
					}
					for (int deg = start_deg - deg_param; deg <= start_deg; deg++) {

						ofVertex(this->location_list[location_index] + glm::vec2(this->size * cos(deg * DEG_TO_RAD), this->size * sin(deg * DEG_TO_RAD)));
					}

					ofEndShape();
				}
			}
		}
		else {

			ofSetLineWidth(3);
			auto distance = glm::distance(satellite_point_next, satellite_point_current);
			distance = ofMap(frame_param, 0, this->frame_span, distance, 0);
			auto direction = satellite_point_next - satellite_point_current;
			ofDrawLine(satellite_point_next, satellite_point_next - glm::normalize(direction) * distance);

			ofSetLineWidth(1.5);
			for (int location_index = 0; location_index < this->location_list.size(); location_index++) {

				ofSetColor(255);
				ofNoFill();
				ofDrawCircle(this->location_list[location_index], this->size);

				if (next_index == location_index) {

					int deg_param = ofMap(frame_param, 0, this->frame_span, 90, 180);
					float start_deg = angle_next * RAD_TO_DEG;

					ofFill();
					ofSetColor(this->color_list[i]);
					ofBeginShape();
					for (int deg = start_deg; deg <= start_deg + deg_param; deg++) {

						ofVertex(this->location_list[location_index] + glm::vec2(this->size * cos(deg * DEG_TO_RAD), this->size * sin(deg * DEG_TO_RAD)));
					}
					for (int deg = start_deg - deg_param; deg <= start_deg; deg++) {

						ofVertex(this->location_list[location_index] + glm::vec2(this->size * cos(deg * DEG_TO_RAD), this->size * sin(deg * DEG_TO_RAD)));
					}

					ofEndShape();
				}
			}
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}