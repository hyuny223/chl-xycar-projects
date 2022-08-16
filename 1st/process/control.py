import numpy as np



class PID():
  def __init__(self,kp,ki,kd):
    self.kp = kp
    self.ki = ki
    self.kd = kd
    self.p_error = 0.0
    self.i_error = 0.0
    self.d_error = 0.0
  def pid_control(self, cte):
    self.d_error = cte-self.p_error
    self.p_error = cte
    self.i_error += cte
    return self.kp*self.p_error + self.ki*self.i_error + self.kd*self.d_error


# 이동 평균 필터
def movAvgFilter(pos, k, preAvg, buf):
    global N
    if k == 0:
        buf = pos*np.ones(N + 1)
        k, preAvg = 1, pos

    for i in range(0, N):
        buf[i] = buf[i + 1]

    buf[N] = pos
    avg = preAvg + (pos - buf[0]) / N
    preAvg = avg
    return k, preAvg, buf, int(round(avg))




def MovingAverage():
    def __init__(self,n):
        self.samples = n
        self.data = []
        self.weights = list(range(1, n+1))

    def add_sample(self, new_sample):
        if len(self.data) < self.samples:
            self.data.append(new_sample)
        else:
            self.data = self.data[1:] + [new_sample]

    def get_mm(self):
        return float(sum(self.data)) / len(self.data)

    def get_wmm(self):
        s=0
        for i, x in enumerate(self.data):
            s += x*self.weights[i]

        return float(s) / sum(self.weights[:len(self.data)])
