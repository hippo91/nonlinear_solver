from .vnr_internal_energy import launch_vnr_resolution, MieGruneisenParams_s

class MieGruneisenParams(MieGruneisenParams_s):
    """
    Just a wrapper class around MieGruneisenParams_s
    """
    def __init__(self, czero, b, ezero, grunzero, rhozero, S1, S2, S3):
        super().__init__()
        self.c_zero = czero
        self.coeff_b = b
        self.e_zero = ezero
        self.gamma_zero = grunzero
        self.rho_zero = rhozero
        self.s1 = S1
        self.s2 = S2
        self.s3 = S3