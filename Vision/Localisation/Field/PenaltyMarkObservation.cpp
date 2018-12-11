#include "PenaltyMarkObservation.hpp"

#include "Field/Field.hpp"
#include "CameraState/CameraState.hpp"

#include "RhIO.hpp"

#include "rhoban_utils/logging/logger.h"

#include "Utils/Interface.h"

static rhoban_utils::Logger out("PenaltyMarkObservation");

using Vision::Utils::CameraState;
using namespace rhoban_geometry;
using namespace rhoban_utils;

namespace Vision {
namespace Localisation {

double PenaltyMarkObservation::pError = 0.2;
double PenaltyMarkObservation::weightRatio = 0.2;
// Angle tol
double PenaltyMarkObservation::maxAngleError = 10;
double PenaltyMarkObservation::tolAngleError = 2;
double PenaltyMarkObservation::similarAngleLimit = 5;
// Cart tol
double PenaltyMarkObservation::maxCartError = 1.0;
double PenaltyMarkObservation::tolCartError = 0.2;
double PenaltyMarkObservation::similarPosLimit = 0.5;



static double getScore(double error, double maxError, double tol) {
  if (error >= maxError) return 0;
  if (error <= tol) return 1;
  return 1 - (error - tol) / (maxError - tol);
}

PenaltyMarkObservation::PenaltyMarkObservation() : PenaltyMarkObservation(Angle(0), Angle(0), 0, 1) {}

PenaltyMarkObservation::PenaltyMarkObservation(const Angle &panToPenaltyMark,
                                               const Angle &tiltToPenaltyMark,
                                               double robotHeight_,
                                 double weight_)
  : pan(panToPenaltyMark), tilt(tiltToPenaltyMark),
    robotHeight(robotHeight_), weight(weight_)
{
}

cv::Point3f PenaltyMarkObservation::getSeenDir() const
{
  cv::Point2f seenPos = CameraState::xyFromPanTilt(pan, tilt, robotHeight);
  return cv::Point3f(seenPos.x, seenPos.y, -robotHeight);
}

double PenaltyMarkObservation::potential(const FieldPosition &p) const {
  return potential(p, false);
}

double PenaltyMarkObservation::potential(const FieldPosition &p, bool debug) const {
  double bestScore = 0;
  cv::Point3f seenDir = getSeenDir();

  std::ostringstream oss;
  if (debug) {
    oss << "Debugging potential for particle (" << p.x() << ", " << p.y()
        << ", " << p.getOrientation().getSignedValue()
        << ") -> seenDir: " << seenDir << std::endl;
  }

  for (auto &penaltyMark : Field::Field::getPenaltyMarks()) {
    Point field_postPos = cv2rg(penaltyMark) - p.getRobotPosition();
    Point robot_postPos = field_postPos.rotation(-p.getOrientation());
    // Rotation of alpha around robotPos
    cv::Point2f expectedPos = rg2cv2f(robot_postPos);
    cv::Point3f expectedDir(expectedPos.x, expectedPos.y, -robotHeight);
    // Computing errors
    double dx = expectedPos.x - seenDir.x;
    double dy = expectedPos.y - seenDir.y;
    double cartDiff = sqrt(dx*dx + dy*dy);
    // aDiff is always positive (angleBetween)
    double aDiff = angleBetween(seenDir, expectedDir).getSignedValue();
    // Computing scores
    double aScore = getScore(aDiff, maxAngleError, tolAngleError);
    double cartScore = getScore(cartDiff, maxCartError, tolCartError);
    double score = std::max(aScore, cartScore);

    if (debug) {
      oss << "\tPost: " << penaltyMark << " -> expDir: " << expectedDir << ", "
          << "angle: (" << aDiff << ", " << aScore << ") "
          << "cartesian: (" << cartDiff << ", " << cartScore << ")" << std::endl;
    }

    // If current post has a better score, use it
    if (score > bestScore) {
      bestScore = score;
    }
  }

  if (debug) {
    out.log("%s", oss.str().c_str());
  }

  return getWeightedScore(bestScore * (1-pError) + pError);
}

void PenaltyMarkObservation::merge(const PenaltyMarkObservation & other)
{
  pan  = Angle::weightedAverage(pan , weight, other.pan , other.weight);
  tilt = Angle::weightedAverage(tilt, weight, other.tilt, other.weight);
  weight = weight + other.weight;
}

bool PenaltyMarkObservation::isSimilar(const PenaltyMarkObservation & o1,
                                const PenaltyMarkObservation & o2)
{
  cv::Point3f seenDir1 = o1.getSeenDir();
  cv::Point3f seenDir2 = o2.getSeenDir();
  Angle aDiff = angleBetween(seenDir1, seenDir2);
  double dx = seenDir1.x - seenDir2.x;
  double dy = seenDir1.y - seenDir2.y;
  double dist = dx*dx + dy*dy;
  bool angleSimilar = aDiff.getValue() < similarAngleLimit;// aDiff is in [0,180]
  bool posSimilar = dist < similarPosLimit;
  return angleSimilar || posSimilar;
}

void PenaltyMarkObservation::bindWithRhIO() {
  RhIO::Root.newFloat("/localisation/field/PenaltyMarkObservation/pError")
      ->defaultValue(pError)
      ->minimum(0.0)
      ->maximum(1.0)
      ->comment("The false positive probability");
  RhIO::Root.newFloat("/localisation/field/PenaltyMarkObservation/maxAngleError")
      ->defaultValue(maxAngleError)
      ->minimum(0.0)
      ->maximum(180)
      ->comment("The maximum difference between expectation and observation [deg]");
  RhIO::Root.newFloat("/localisation/field/PenaltyMarkObservation/tolAngleError")
      ->defaultValue(tolAngleError)
      ->minimum(0.0)
      ->maximum(10)
      ->comment("The tolerance between expectation and observation [deg]");
  RhIO::Root.newFloat("/localisation/field/PenaltyMarkObservation/similarAngleLimit")
      ->defaultValue(similarAngleLimit)
      ->minimum(0.0)
      ->maximum(90.0)
      ->comment("Maximal angular difference for similar observations (merge) [deg]");
  RhIO::Root.newFloat("/localisation/field/PenaltyMarkObservation/maxCartError")
      ->defaultValue(maxCartError)
      ->minimum(0.0)
      ->maximum(180)
      ->comment("The maximum difference between expectation and observation [m]");
  RhIO::Root.newFloat("/localisation/field/PenaltyMarkObservation/tolCartError")
      ->defaultValue(tolCartError)
      ->minimum(0.0)
      ->maximum(10)
      ->comment("The tolerance between expectation and observation [m]");
  RhIO::Root.newFloat("/localisation/field/PenaltyMarkObservation/similarCartLimit")
      ->defaultValue(similarPosLimit)
      ->minimum(0.0)
      ->maximum(90.0)
      ->comment("Maximal position difference for similar observations (merge) [m]");
  RhIO::Root.newFloat("/localisation/field/PenaltyMarkObservation/weightRatio")
    ->defaultValue(weightRatio)
    ->minimum(0.0)
    ->maximum(90.0)
    ->comment("How is score growing with several particles? pow(score,1+weight*weightRatio)");
}

void PenaltyMarkObservation::importFromRhIO() {
  RhIO::IONode &node = RhIO::Root.child("localisation/field/PenaltyMarkObservation");
  pError = node.getValueFloat("pError").value;
  maxAngleError = node.getValueFloat("maxAngleError").value;
  similarAngleLimit = node.getValueFloat("similarAngleLimit").value;
  similarPosLimit = node.getValueFloat("similarCartLimit").value;
}

std::string PenaltyMarkObservation::getClassName() const { return "PenaltyMarkObservation"; }

Json::Value PenaltyMarkObservation::toJson() const {
  Json::Value v;
  v["robotHeight"] = robotHeight;
  v["pan"] = pan.getSignedValue();
  v["tilt"] = tilt.getSignedValue();
  return v;
}

void PenaltyMarkObservation::fromJson(const Json::Value & v, const std::string & dir_name) {
  (void) dir_name;
  rhoban_utils::tryRead(v,"robotHeight",&robotHeight);
  rhoban_utils::tryRead(v,"pan",&pan);
  rhoban_utils::tryRead(v,"tilt",&tilt);
}

double PenaltyMarkObservation::getMinScore() const {
  return getWeightedScore(pError);
}

double PenaltyMarkObservation::getWeightedScore(double score) const {
  return pow(score, 1 + (weight-1) * weightRatio);
}

std::string PenaltyMarkObservation::toStr() const {
  std::ostringstream oss;
  oss << "[PenaltyMarkObservation: pan=" << pan.getSignedValue()
      << "° tilt=" << tilt.getSignedValue() << "°]";
  return oss.str();
}

}
}
