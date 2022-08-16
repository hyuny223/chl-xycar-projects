import cv2
import numpy as np

class Kalman:
  kf = cv2.KalmanFilter(4,2)
  kf.measurementMatrix = np.array([[1,0,0,0],[0,1,0,0]], np.float32)
  kf.transitionMatrix = np.array([[1,0,1,0],[0,1,0,1],[0,0,1,0],[0,0,0,1]], np.float32)
  kf.processNoiseCov = np.array([[1,0,0,0],[0,1,0,0],[0,0,1,0],[0,0,0,1]],np.float32) * (1e-4) #* (1e-5) # process noise covariance matrix (Q)
  '''
  파라미터에 따라 곡선 예측 또는 직선 예측을 잘 하는 값이 있는데, 반비례관계인가..?
  '''
  kf.measurementNoiseCov = np.array([[1,0],[0,1]],np.float32) # measurement noise covariance matrix (R). [1,0],[0,1] is the default value anyway…
  kf.statePost = np.array([[66],[514]],np.float32) # corrected state (x(k)): x(k)=x'(k)+K(k)*(z(k)-H*x'(k))
  # 66, 514

  def predict(self, coordX, coordY):
    measured = np.array([[np.float32(coordX)], [np.float32(coordY)]])
    self.kf.correct(measured) # Updates the predicted state from the measurement.
    predicted = self.kf.predict()
    x,y = int(predicted[0]), int(predicted[1])
    return x, y
