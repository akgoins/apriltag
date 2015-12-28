#ifndef APRILTAGS_TAGDETECTION_H_
#define APRILTAGS_TAGDETECTION_H_

#include <Eigen/Dense>
#include <opencv2/core/core.hpp>

#include "AprilTags/TagCodes.h"

#include <utility>
#include <vector>

namespace AprilTags {

using Pointf = std::pair<float, float>;

struct TagDetection {
  TagDetection() = default;
  TagDetection(unsigned id, bool good, code_t obs_code, code_t code,
               unsigned hamming_distance, unsigned num_rotations)
      : id(id),
        good(good),
        obs_code(obs_code),
        code(code),
        hamming_distance(hamming_distance),
        num_rotations(num_rotations) {}

  unsigned id;

  bool good = false;

  /**
   * @brief obs_code Observed code
   */
  code_t obs_code;

  /**
   * @brief code Matched code
   */
  code_t code;

  /**
   * @brief hamming_distance
   */
  unsigned hamming_distance;

  /**
   * @brief num_rotations Number of 90 degree rotations clockwise required to
   * align the code
   */
  unsigned num_rotations;

  /////////////// Fields below are filled in by TagDetector ///////////////

  /**
   * @brief cxy Center of tag in pixel coordinates
   */
  cv::Point2f cxy;

  /**
   * @brief p Position of the detection
   * The points travel counter-clockwise around the target, alwasy starting from
   * the same corner of the tag
   */
  cv::Point2f p[4];

  /**
   * @brief obs_perimeter length of the observed perimeter
   * Observed perimeter excludes the inferred perimeter which is used to connect
   * incomplete quads
   */
  float obs_perimeter;

  //! A 3x3 homography that computes pixel coordinates from tag-relative
  // coordinates.
  /*  Both the input and output coordinates are 2D homogeneous vectors, with y =
   * Hx.
   *  'y' are pixel coordinates, 'x' are tag-relative coordinates. Tag
   * coordinates span
   *  from (-1,-1) to (1,1). The orientation of the homography reflects the
   * orientation
   *  of the target.
   */
  Eigen::Matrix3d H;

  //! The homography is relative to image center, whose coordinates are below.
  std::pair<float, float> hxy;

  //! Interpolate point given (x,y) is in tag coordinate space from (-1,-1) to
  //(1,1).
  std::pair<float, float> interpolate(float x, float y) const;

  //! Used to eliminate redundant tags
  bool OverlapsTooMuch(const TagDetection& other) const;

  //! Scale this tag
  // TODO: Also need to scale homography?
  void scaleTag(float scale);

  //! Orientation in the xy plane
  //  float getXYOrientation() const;

  //! Relative pose of tag with respect to the camera
  /* Returns the relative location and orientation of the tag using a
     4x4 homogeneous transformation matrix (see Hartley&Zisserman,
     Multi-View Geometry, 2003). Requires knowledge of physical tag
     size (side length of black square in meters) as well as camera
     calibration (focal length and principal point); Result is in
     camera frame (z forward, x right, y down)
  */
  //  Eigen::Matrix4d getRelativeTransform(double tag_size, double fx, double
  //  fy,
  //                                       double px, double py) const;

  //! Recover rotation matrix and translation vector of April tag relative to
  // camera.
  // Result is in object frame (x forward, y left, z up)
  //  void getRelativeTranslationRotation(double tag_size, double fx, double fy,
  //                                      double px, double py,
  //                                      Eigen::Vector3d& trans,
  //                                      Eigen::Matrix3d& rot) const;

  //! Better version
  //  Eigen::Matrix4d getRelativeH(double tag_size, const cv::Matx33d& K,
  //                               const cv::Mat_<double>& D) const;
  //  void getRelativeQT(double tag_size, const cv::Matx33d& K,
  //                     const cv::Mat_<double>& D, Eigen::Quaterniond& quat,
  //                     Eigen::Vector3d& trans) const;
  //  void getRelativeRT(double tag_size, const cv::Matx33d& K,
  //                     const cv::Mat_<double>& D, cv::Mat& rvec,
  //                     cv::Mat& tvec) const;
};

}  /// namespace AprilTags

#endif  // APRILTAGS_TAGDETECTION_H_
