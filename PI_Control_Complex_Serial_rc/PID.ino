
float calculate_PID() {
  //run every 10 ms

  int32_t error = setAng-angle;

  P =  error;

  I +=  error * 0.010;

  D = (error-prev_error);

  float PID =  k_p *P + k_i *I + k_d * D;
  prev_error = error;

  return PID;
}

float calculate_PIDv() {
  //run every 10 ms

 int32_t errorv = setVel-vel;

  Pv =  errorv;

  Iv +=  errorv * 0.010;

  Dv = (errorv-prev_errorv);

  float PIDv = k_pv *Pv + k_iv *Iv + k_dv * Dv;

  prev_errorv = errorv;

  return PIDv;
}
 

