/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>
#include <vector>

#include "testing/gtest/include/gtest/gtest.h"
#include "bat/ads/internal/filters/ads_history_confirmation_filter.h"
#include "bat/ads/internal/client_mock.h"
#include "bat/ads/internal/ads_client_mock.h"
#include "bat/ads/internal/ads_impl.h"

// npm run test -- brave_unit_tests --filter=BatAds*

using std::placeholders::_1;
using ::testing::_;
using ::testing::Invoke;

namespace ads {

class BatAdsHistoryConfirmationFilterTest : public ::testing::Test {
 protected:
  BatAdsHistoryConfirmationFilterTest()
      : mock_ads_client_(std::make_unique<MockAdsClient>()),
        ads_(std::make_unique<AdsImpl>(mock_ads_client_.get())) {
    // You can do set-up work for each test here
  }

  ~BatAdsHistoryConfirmationFilterTest() override {
    // You can do clean-up work that doesn't throw exceptions here
  }

  // If the constructor and destructor are not enough for setting up and
  // cleaning up each test, you can use the following methods

  void SetUp() override {
    // Code here will be called immediately after the constructor (right before
    // each test)

    auto callback = std::bind(
        &BatAdsHistoryConfirmationFilterTest::OnAdsImplInitialize, this, _1);
    ads_->Initialize(callback);

    ads_history_filter_ = std::make_unique<AdsHistoryConfirmationFilter>();
  }

  void OnAdsImplInitialize(const Result result) {
    EXPECT_EQ(Result::SUCCESS, result);
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right before the
    // destructor)
  }

  bool CompareUnsortedAdsHistory(
      const std::deque<AdHistory> a,
      const std::deque<AdHistory> b) const {
    const size_t n = a.size();

    if (b.size() != n) {
      return false;
    }

    std::vector<bool> visited(n, false);

    size_t j;
    for (size_t i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        if (a[i] == b[j] && !visited[j]) {
            visited[j] = true;
            break;
        }
      }

      if (j == n) {
        return false;
      }
    }

    return true;
  }

  std::unique_ptr<MockAdsClient> mock_ads_client_;
  std::unique_ptr<AdsImpl> ads_;

  std::unique_ptr<AdsHistoryFilter> ads_history_filter_;
};

TEST_F(BatAdsHistoryConfirmationFilterTest,
    FilterActions) {
  // Arrange
  AdHistory ad1;
  ad1.parent_uuid = "ab9deba5-01bf-492b-9bb8-7bc4318fe272";  // Ad 1 (Viewed)
  ad1.ad_content.ad_action = ConfirmationType::kViewed;

  AdHistory ad2;
  ad2.parent_uuid = "a577e7fe-d86c-4997-bbaa-4041dfd4075c";  // Ad 2 (Viewed)
  ad2.ad_content.ad_action = ConfirmationType::kViewed;

  AdHistory ad3;
  ad3.parent_uuid = "ab9deba5-01bf-492b-9bb8-7bc4318fe272";  // Ad 1 (Clicked)
  ad3.ad_content.ad_action = ConfirmationType::kClicked;

  AdHistory ad4;
  ad4.parent_uuid = "4424ff92-fa91-4ca9-a651-96b59cf1f68b";  // Ad 3 (Dismissed)
  ad4.ad_content.ad_action = ConfirmationType::kDismissed;

  AdHistory ad5;
  ad5.parent_uuid = "4424ff92-fa91-4ca9-a651-96b59cf1f68b";  // Ad 3 (Viewed)
  ad5.ad_content.ad_action = ConfirmationType::kViewed;

  AdHistory ad6;
  ad6.parent_uuid = "d9253022-b023-4414-a85d-96b78d36435d";  // Ad 4 (Viewed)
  ad6.ad_content.ad_action = ConfirmationType::kViewed;

  const std::deque<AdHistory> ads_history = {
    ad1,
    ad2,
    ad3,
    ad4,
    ad5,
    ad6
  };

  // Act
  const std::deque<AdHistory> ads_history_filtered =
      ads_history_filter_->Apply(ads_history);

  // Assert
  const std::deque<AdHistory> expected_ads_history = {
    ad2,  // Ad 2
    ad3,  // Ad 1 (Click) which should supercede Ad 1 (View)
    ad4,  // Ad 3 (Dismiss) which should supercede Ad 3 (View)
    ad6   // Ad 4
  };

  EXPECT_TRUE(CompareUnsortedAdsHistory(expected_ads_history,
      ads_history_filtered));
}

TEST_F(BatAdsHistoryConfirmationFilterTest,
    FilterUnsupportedActions) {
  // Arrange
  AdHistory ad1;
  ad1.parent_uuid = "36c24bef-eaee-4507-bad6-15612dec1273";
  ad1.ad_content.ad_action = ConfirmationType::kUnknown;      // Unsupported

  AdHistory ad2;
  ad2.parent_uuid = "69b684d7-d893-4f4e-b156-859919a0fcc9";
  ad2.ad_content.ad_action = ConfirmationType::kLanded;       // Unsupported

  AdHistory ad3;
  ad3.parent_uuid = "d3be2e79-ffa8-4b4e-b61e-88545055fbad";
  ad3.ad_content.ad_action = ConfirmationType::kFlagged;      // Unsupported

  AdHistory ad4;
  ad4.parent_uuid = "9390f66a-d4f2-4c8a-8315-1baed4aae612";
  ad4.ad_content.ad_action = ConfirmationType::kUpvoted;      // Unsupported

  AdHistory ad5;
  ad5.parent_uuid = "47c73793-d1c1-4fdb-8530-4ae478c79783";
  ad5.ad_content.ad_action = ConfirmationType::kDownvoted;    // Unsupported

  AdHistory ad6;
  ad6.parent_uuid = "b7e1314c-73b0-4291-9cdd-6c5d2374c28f";
  ad6.ad_content.ad_action = ConfirmationType::kConversion;   // Unsupported

  AdHistory ad7;
  ad7.parent_uuid = "ab9deba5-01bf-492b-9bb8-7bc4318fe272";   // Ad 1 (View)
  ad7.ad_content.ad_action = ConfirmationType::kViewed;

  const std::deque<AdHistory> ads_history = {
    ad1,
    ad2,
    ad3,
    ad4,
    ad5,
    ad6,
    ad7
  };

  // Act
  const std::deque<AdHistory> ads_history_filtered =
      ads_history_filter_->Apply(ads_history);

  // Assert
  const std::deque<AdHistory> expected_ads_history = {
    ad7  // Ad 1 (View)
  };

  EXPECT_TRUE(CompareUnsortedAdsHistory(expected_ads_history,
      ads_history_filtered));
}

}  // namespace ads
