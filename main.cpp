#include <Novice.h>
#include <Novice.h>
#include <imgui.h>
#include "MakeMatrix.h"
#include "MatrixMath.h"
#include "Vector3Math.h"
#include "Draw.h"
#include "algorithm"

const char kWindowTitle[] = "LE2B_17_ナガイ_コハク_MT3_2_7 AABBと線の当たり判定";

bool IsCollision(AABB aabb, Segment segment);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//ウィンドウサイズ
	float kWindowWidth = 1280.0f;
	float kWindowHeight = 720.0f;

	//カメラ:平行移動
	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };

	//カメラ:回転
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	AABB aabb{
		.min{-0.5f,-0.5f,-0.5f},
		.max{0.5f,0.5f,0.5f}
	};

	Segment segment{
		.origin{-0.7f,0.3f,-0.5f},
		.diff{2.0f,-0.5f,0.0f}
	};

	Draw draw;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		if (ImGui::TreeNode("Camera")) {
			ImGui::DragFloat3("Translate", &cameraTranslate.x, 0.01f);
			ImGui::DragFloat3("Rotate", &cameraRotate.x, 0.01f);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("AABB")) {
			ImGui::DragFloat3("min", &aabb.min.x, 0.01f);
			ImGui::DragFloat3("max", &aabb.max.x, 0.01f);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Sphere")) {
			ImGui::DragFloat3("origin", &segment.origin.x, 0.01f);
			ImGui::DragFloat3("diff", &segment.diff.x, 0.01f);
			ImGui::TreePop();
		}

		aabb.min.x = (std::min)(aabb.min.x, aabb.max.x);
		aabb.min.y = (std::min)(aabb.min.y, aabb.max.y);
		aabb.min.z = (std::min)(aabb.min.z, aabb.max.z);
		aabb.max.x = (std::max)(aabb.min.x, aabb.max.x);
		aabb.max.y = (std::max)(aabb.min.y, aabb.max.y);
		aabb.max.z = (std::max)(aabb.min.z, aabb.max.z);

		draw.Pipeline(cameraTranslate, cameraRotate, kWindowWidth, kWindowHeight);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		draw.DrawGrid();

		if (IsCollision(aabb,segment)) {
			draw.DrawAABB(aabb, RED);
		} else {
			draw.DrawAABB(aabb, WHITE);
		}

		draw.DrawLine(segment.origin,segment.diff, WHITE);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

bool IsCollision(AABB aabb, Segment segment) {

	float tXMin = (aabb.min.x - segment.origin.x) / segment.diff.x;
	float tXMax = (aabb.max.x - segment.origin.x) / segment.diff.x;

	float tYMin = (aabb.min.y - segment.origin.y) / segment.diff.y;
	float tYMax = (aabb.max.y - segment.origin.y) / segment.diff.y;

	float tZMin = (aabb.min.z - segment.origin.z) / segment.diff.z;
	float tZMax = (aabb.max.z - segment.origin.z) / segment.diff.z;

	float tNearX = fminf(tXMin, tXMax);
	float tNearY = fminf(tYMin, tYMax);
	float tNearZ = fminf(tZMin, tZMax);

	float tFarX = fmaxf(tXMin, tXMax);
	float tFarY = fmaxf(tYMin, tYMax);
	float tFarZ = fmaxf(tZMin, tZMax);

	float tMin = fmaxf(fmaxf(tNearX, tNearY), tNearZ);
	float tMax = fminf(fminf(tFarX, tFarY), tFarZ);

	if (tMin <= tMax) {
		return true;
	}

	return false;
}