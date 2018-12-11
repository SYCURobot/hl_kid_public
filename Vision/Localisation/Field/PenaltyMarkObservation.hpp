#pragma once

#include "Localisation/Field/SerializableFieldObservation.hpp"
#include "CameraState/CameraState.hpp"

namespace Vision {
namespace Localisation {

class PenaltyMarkObservation : public SerializableFieldObservation {
public:
  rhoban_utils::Angle pan;
  rhoban_utils::Angle tilt;

  /// [m]
  double robotHeight;

  /// Several PenaltyMark observations can be merged into a single one,
  /// In this case, the weight of the observation is increased because
  /// observations are consistent
  double weight;

  /// Minimal score for an observation of weight 1
  static double pError;

  /// [deg]
  static double maxAngleError;
  static double tolAngleError;

  /// Limit used to determine if observations are similars
  static double similarAngleLimit;

  /// [m]
  static double maxCartError;
  static double tolCartError;

  /// Limit used to determine if observations are similars
  static double similarPosLimit;

  /// Used to weight more observations which represent large clusters
  static double weightRatio;

public:
  PenaltyMarkObservation();

  /**
   * toPenaltyMark angle is given in robot referential (left = -, right = +)
   */
  PenaltyMarkObservation(const rhoban_utils::Angle &panToPenaltyMark,
                         const rhoban_utils::Angle &tiltToPenaltyMark,
                         double robotHeight, double weight = 1);

  cv::Point3f getSeenDir() const;

  virtual double potential(const FieldPosition &p) const override;

  double potential(const FieldPosition &p, bool debug) const;


  /// Merge 'other' observation into current one:
  /// average angles and cumulate weights
  void merge(const PenaltyMarkObservation & other);

  static bool isSimilar(const PenaltyMarkObservation & o1,
                        const PenaltyMarkObservation & o2);

  static void bindWithRhIO();
  static void importFromRhIO();

  std::string getClassName() const override;
  Json::Value toJson() const override;
  void fromJson(const Json::Value & v, const std::string & dir_name) override;

  double getMinScore() const override;

  double getWeightedScore(double score) const;

  virtual std::string toStr() const override;
};
}
}
