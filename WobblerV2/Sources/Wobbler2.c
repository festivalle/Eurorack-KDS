#include "Wobbler2.h"
#include <Math.h>
#include "../../EurorackShared/EURORACKSHARED.H"

#define __max(a,b) ((a)>(b)?(a):(b))
#define __min(a,b) ((a)<(b)?(a):(b))

#ifdef __cplusplus
extern "C"
{
#endif

	void Wobbler2_RandomSeed(struct Wobbler2_RandomGen *R, unsigned int seed)
	{
		R->RandomMemory = (long)seed;
	}

	void Wobbler2_SyncPulse(struct Wobbler2_LFO_t *LFO)
	{
		int Delta = LFO->timesincesync;
		if (Delta > 3000)
		{
			LFO->syncindex = 0;

		}
		else
		{
			LFO->syncindex++;
		}
		if (LFO->syncindex > 2)
		{
			LFO->synctime[0] = LFO->synctime[1];
			LFO->synctime[1] = LFO->synctime[2];
			LFO->syncindex = 2;
		}
		LFO->synctime[LFO->syncindex] = Delta;

	}

	int Wobbler2_Rand(struct Wobbler2_RandomGen *R)
	{
		return (((R->RandomMemory = R->RandomMemory * 214013L + 2531011L) >> 16) & 0x7fff);
	}

	void Wobbler2_Init(struct Wobbler2_LFO_t *LFO)
	{
		LFO->Output = 0;
		LFO->OutputPhased = 0;
		LFO->Phase1 = 0;
		LFO->Gate[0] = 0;
		LFO->Gate[1] = 0;
		LFO->EnvelopeVal = 0;
		LFO->PhasedCountdown = 0;
		LFO->EnvelopeState = Wobbler2_IDLE;
		Wobbler2_InitPendulum(&LFO->Pendulum);
	}

	void Wobbler2_Trigger(struct Wobbler2_LFO_t *LFO, unsigned char N, struct Wobbler2_Params *Params)
	{
		if (N == 0)
		{
			LFO->Phase1 = 0;
			Wobbler2_StartTwang(LFO);
		}
	}

	void Wobbler2_LoadSettings(struct Wobbler2_Settings *settings, struct Wobbler2_Params *params)
	{

	}

	void Wobbler2_ValidateParams(struct Wobbler2_Params *params)
	{

	}

	unsigned long Wobbler2_LFORange(int32_t V, int32_t SR)
	{
		return  1 + ((V*SR * 64)>>8);
		//	return (unsigned long)(64 * pow((int32_t)((SR * 6) / 64.0), pow((int32_t)V, 0.54f)));
	}

	unsigned long Wobbler2_LFORange2(int32_t V)
	{
		return  1 + V*V *8;
		//	return (unsigned long)(64 * pow((int32_t)((SR * 6) / 64.0), pow((int32_t)V, 0.54f)));
	}



	void Wobbler2_InitPendulum(Wobbler2_Pendulum_t *P)
	{
		P->A = 0;
		P->B = 0;
		P->m1 = 1;
		P->m2 = 1;
		P->Theta1 = 3.1415 / 2.0f;
		P->Theta2 = 3.1415 / 2.0f;
		P->d2Theta1 = 0;
		P->d2Theta2 = 0;
		P->dTheta1 = 0;
		P->dTheta2 = 0;
		P->l1 = 0.2;
		P->l2 = 0.2;
		P->g = .981;
		P->mu = 1 + P->m1 / P->m2;
	}

	float fsin(float P)
	{
	//	P *= (float)(1 << 32);
		P /= (float)6.283f;;
		P *= (float)0xffffffff;
		int32_t phase = (int)P;
		int32_t R = Sine(phase);
		return (float)R / (float)(1 << 30);
	}
	void Wobbler2_DoublePendulum( Wobbler2_Pendulum_t *P, float DT)
	{
		//while (P->Theta1 < 0) { P->Theta1 += 6.283f; P->Theta2 += 6.283f; }
		float st1 = sin(P->Theta1);
		float st2 = sin(P->Theta2);
		float _2sub1 =P->Theta2 - P->Theta1;
		float _1sub2 =P->Theta1 - P->Theta2;
		float c1sub2 = cos(_1sub2);
		float s1sub2 = sin(_1sub2);
		P->d2Theta1 = (P->g*(st2*c1sub2 - P->mu*st1) - (P->l2*P->dTheta2*P->dTheta2 + P->l1*P->dTheta1*P->dTheta1*c1sub2)*s1sub2) / (P->l1*(P->mu - c1sub2*c1sub2));
		P->d2Theta2 = (P->mu*P->g*(st1*c1sub2 - st2) + (P->mu*P->l1*P->dTheta1*P->dTheta1 + P->l2*P->dTheta2*P->dTheta2*c1sub2)*s1sub2) / (P->l2*(P->mu - c1sub2*c1sub2));
	//	P->dTheta1 *= 0.999;
	//	P->dTheta2 *= 0.999;
		P->dTheta1 += P->d2Theta1*DT;
		P->dTheta2 += P->d2Theta2*DT;
		P->Theta1 += P->dTheta1*DT;
		P->Theta2 += P->dTheta2*DT;
		P->A = P->Theta1 * 0xffff;
		P->B = P->Theta2 * 0xffff;
	}


#define FIXBITS 16

#define F(x) ((int)(x * (1<<FIXBITS)))
	void Wobbler2_InitIntPendulum( SpringMassSystem_t *P)
	{
		P->MassCount = 3;
		P->SpringCount = 2;
		for (int i = 0; i < P->MassCount;i++)
		{
			P->Masses[i].Force.X = 0;
			P->Masses[i].Force.Y = 0;
			P->Masses[i].Speed.X = 0;
			P->Masses[i].Speed.Y = 0;
			P->Masses[i].Pos.X = 0;
			P->Masses[i].Pos.Y = 0;
		}
		
		P->Masses[0].fixed = 1;
		P->Masses[0].mass = F(1);

		P->Masses[1].fixed = 0;
		P->Masses[1].mass = F(1);
		P->Masses[1].Pos.X = F(40);
		P->Masses[1].Pos.Y = F(0);
		P->Masses[1].Speed.Y = F(20);

		P->Masses[2].fixed = 0;
		P->Masses[2].mass = F(1);
		P->Masses[2].Pos.Y = F(0);
		P->Masses[2].Pos.X = F(80);

		P->Springs[0].A = 0;
		P->Springs[0].B = 1;
		P->Springs[0].K = F(0.5);
		P->Springs[0].RestLength = F(40);


		P->Springs[1].A = 1;
		P->Springs[1].B = 2;
		P->Springs[1].K = F(0.5);
		P->Springs[1].RestLength = F(40);
	}

	typedef struct State
	{
		int32_t m_position;
		int32_t m_velocity;
	} State;

	int32_t motionFunction(State *t, int32_t dt)
	{

	}

	State evaluate0( State *_initial)
	{

		State output;
		output.m_position = _initial->m_velocity;
		output.m_velocity = motionFunction(_initial, 0);
		return output;
	}
	
	State evaluate( State *_initial, int32_t _dt, const State *_d)
	{
		State state;
		state.m_position = _initial->m_position + imul(_d->m_position,_dt);
		state.m_velocity = _initial->m_velocity + imul(_d->m_velocity,_dt);
		State output;
		output.m_position = state.m_velocity;
		output.m_velocity = motionFunction(&state,  _dt);
		return output;
	}
	
	void integrate(State *m_state, int32_t dt)
	{
		int32_t hdt = imul(dt, F(0.5));
		State a = evaluate0(m_state);
		State b = evaluate (m_state,  hdt, &a);
		State c = evaluate (m_state,  hdt, &b);
		State d = evaluate (m_state,   dt, &c);
		const int32_t dxdt = imul(F( 1.0f / 6.0f ),  (a.m_position + 2 * (b.m_position + c.m_position) + d.m_position));
		const int32_t dvdt = imul(F( 1.0f / 6.0f ),  (a.m_velocity + 2 * (b.m_velocity + c.m_velocity) + d.m_velocity));

		m_state->m_position = m_state->m_position + imul(dxdt,dt);
		m_state->m_velocity = m_state->m_velocity + imul(dvdt,dt);
	}



	/* Computing the number of leading zeros in a word. */
	static int32_tclz(uint32_t x)
	{
		int32_t n;
	
		/* See "Hacker's Delight" book for more details */
		if (x == 0) return 32;
		n = 0;
		if (x <= 0x0000FFFF) { n = n + 16; x = x << 16; }
		if (x <= 0x00FFFFFF) { n = n + 8; x = x << 8; }
		if (x <= 0x0FFFFFFF) { n = n + 4; x = x << 4; }
		if (x <= 0x3FFFFFFF) { n = n + 2; x = x << 2; }
		if (x <= 0x7FFFFFFF) { n = n + 1; }

		return n;
	}

	static unsigned fp_sqrt(unsigned val)
	{
		unsigned x;
		int bitpos;
		unsigned long long v;

		if (!val)
			return val;

		/* clz = count-leading-zeros. bitpos is the position of the most significant bit,
		relative to "1" or 1 << base */
		bitpos = FIXBITS - int32_tclz(val);

		/* Calculate our first estimate.
		We use the identity 2^a * 2^a = 2^(2*a) or:
		sqrt(2^a) = 2^(a/2)
		*/
		if (bitpos > 0u) /* val > 1 */
			x = (1u << FIXBITS) << (bitpos >> 1u);
		else if (bitpos < 0) /* 0 < val < 1 */
			x = (1u << FIXBITS) << ((unsigned)(-bitpos) << 1u);
		else /* val == 1 */
			x = (1u << FIXBITS);

		/* We need to scale val with base due to the division.
		Also val /= 2, hence the subtraction of one*/
		v = (unsigned long long)val << (FIXBITS - 1u);

		/* The actual iteration */
		x = (x >> 1u) + v / x;
		x = (x >> 1u) + v / x;
		x = (x >> 1u) + v / x;
		x = (x >> 1u) + v / x;
		return x;
	}
	
	int32_t imul(int32_t a, int32_t b)
	{
		int64_t R = (int64_t)a * (int64_t)b;


		return R >> (FIXBITS);
	}

	int32_t idiv(int32_t a, int32_t b)
	{
		int64_t R = (int64_t)a;
		R << FIXBITS;
		R/= (int64_t)b;
		return R ;
	}

	void UpdateSystem(struct SpringMassSystem_t *P)
	{
		for (int i = 0; i < P->MassCount; i++)
		{
			P->Masses[i].Force.X = 0;
			P->Masses[i].Force.Y = F(9.81);
		}
		
		for (int i = 0; i < P->SpringCount; i++)
		{
			// calc distances
			Mass_t *A = &P->Masses[P->Springs[i].A];
			Mass_t *B = &P->Masses[P->Springs[i].B];
			
			int32_t dx = B->Pos.X - A->Pos.X;
			int32_t dy = B->Pos.Y - A->Pos.Y;
			
			uint32_t lengthsquared = imul(dx, dx) + imul(dy, dy);

			int32_t len = fp_sqrt(lengthsquared);


			if (len > 0)
			{
				int32_t extension = P->Springs[i].RestLength - len;
				State St;
				St.m_position = extension;

				int32_t dxnorm = idiv(dx, len);
				int32_t dynorm = idiv(dy, len);
				float dxf = dx / (float)(1 << FIXBITS);
				float dyf = dy / (float)(1 << FIXBITS);
				float lenf = len / (float)(1 << FIXBITS);
				float extf = extension / (float)(1 << FIXBITS);;
				extf *= 1;
				dxf /= lenf;
				dyf /= lenf;
				if (A->fixed == 1) // add double to B
				{
					B->Force.X += (dxf * extf * 2)*(float)(1<<FIXBITS);
					B->Force.Y += (dyf * extf * 2)*(float)(1 << FIXBITS);
				}
				else
				{
					if (B->fixed == 1) // subtract double from A
					{
						A->Force.X -= (dxf * extf *2)*(float)(1 << FIXBITS);
						A->Force.Y -= (dyf * extf * 2)*(float)(1 << FIXBITS);
					}
					else
					{
						// distribute evenly.
						B->Force.X += (dxf * extf)*(float)(1 << FIXBITS);
						B->Force.Y += (dyf * extf)*(float)(1 << FIXBITS);
						A->Force.X -= (dxf * extf)*(float)(1 << FIXBITS);
						A->Force.Y -= (dyf * extf)*(float)(1 << FIXBITS);
					}
				}
			}
		}

		for (int i = 0; i < P->MassCount; i++)
		{
			if (P->Masses[i].fixed == 0)
			{
				P->Masses[i].Speed.X = imul(F(0.998), P->Masses[i].Speed.X);
				P->Masses[i].Speed.Y = imul(F(0.998), P->Masses[i].Speed.Y);
				P->Masses[i].Speed.X += imul(F(0.1), P->Masses[i].Force.X);
				P->Masses[i].Speed.Y += imul(F(0.1), P->Masses[i].Force.Y);
				P->Masses[i].Pos.X += P->Masses[i].Speed.X;
				P->Masses[i].Pos.Y += P->Masses[i].Speed.Y;
			}
			else
			{
				P->Masses[i].Speed.X = 0;
				P->Masses[i].Speed.Y = 0;
			}
		}
	}

	/**
	* 16-bit fixed point four-quadrant arctangent. Given some Cartesian vector
	* (x, y), find the angle subtended by the vector and the positive x-axis.
	*
	* The value returned is in units of 1/65536ths of one turn. This allows the use
	* of the full 16-bit unsigned range to represent a turn. e.g. 0x0000 is 0
	* radians, 0x8000 is pi radians, and 0xFFFF is (65535 / 32768) * pi radians.
	*
	* Because the magnitude of the input vector does not change the angle it
	* represents, the inputs can be in any signed 16-bit fixed-point format.
	*
	* @param y y-coordinate in signed 16-bit
	* @param x x-coordinate in signed 16-bit
	* @return angle in (val / 32768) * pi radian increments from 0x0000 to 0xFFFF
	*/
/*#define M_1_PI 0.31830988618379067154 

	uint16_t fxpt_atan2(const int16_t y, const int16_t x) {
		if (x == y) { // x/y or y/x would return -1 since 1 isn't representable
			if (y > 0) { // 1/8
				return 8192;
			}
			else if (y < 0) { // 5/8
				return 40960;
			}
			else { // x = y = 0
				return 0;
			}
		}
		const int16_t nabs_y = s16_nabs(y), nabs_x = s16_nabs(x);
		if (nabs_x < nabs_y) { // octants 1, 4, 5, 8
			const int16_t y_over_x = q15_div(y, x);
			const int16_t correction = q15_mul(q15_from_double(0.273 * M_1_PI), s16_nabs(y_over_x));
			const int16_t unrotated = q15_mul(q15_from_double(0.25 + 0.273 * M_1_PI) + correction, y_over_x);
			if (x < 0) { // octants 1, 8
				return unrotated;
			}
			else { // octants 4, 5
				return 32768 + unrotated;
			}
		}
		else { // octants 2, 3, 6, 7
			const int16_t x_over_y = q15_div(x, y);
			const int16_t correction = q15_mul(q15_from_double(0.273 * M_1_PI), s16_nabs(x_over_y));
			const int16_t unrotated = q15_mul(q15_from_double(0.25 + 0.273 * M_1_PI) + correction, x_over_y);
			if (y > 0) { // octants 2, 3
				return 16384 - unrotated;
			}
			else { // octants 6, 7
				return 49152 - unrotated;
			}
		}
	}
	*/
	void Wobbler2_DoublePendulumInt(struct SpringMassSystem_t *P)
	{
		UpdateSystem(P);
	}

	void Wobbler2_StartTwang(struct Wobbler2_LFO_t *LFO)
	{
		//LFO->EnvelopeVal = 0;
		LFO->EnvelopeState = Wobbler2_ATTACK;
	}

	int Wobbler2_SampleHold( Wobbler2_LFO_SNH_t *sh, struct Wobbler2_LFO_t *lfo, uint32_t phase, uint16_t mod)
	{
		int newseg = (phase >> 29);
		SetSVF(&sh->filt, 0x10 + mod / 4, 0x150);

		if (newseg != sh->lastseg)
		{
			if (newseg == 0)
			{
				Wobbler2_RandomSeed(&sh->random, lfo->Phasing);
			}
			sh->lastseg = newseg;
			sh->lastval = (Wobbler2_Rand(&sh->random) << 14) - (1 << 28);
		}
		ProcessSVF(&sh->filt, sh->lastval >> 16);
		return sh->filt.lo;
	}

	int Wobbler2_Twang(struct Wobbler2_LFO_t *LFO, uint32_t phase)
	{
		return (Sine(phase) >> 16) * (LFO->EnvelopeVal >> 8);
	}

	int Wobbler2_Get( Wobbler2_LFO_t *LFO, struct Wobbler2_Params *Params)
	{
		LFO->timesincesync++;
		
		if (LFO->timesincesync > 30000 || LFO->syncindex == 0)
		{
			LFO->extsync = 0;
		}
		else
		{
			LFO->extsync = 1;
		}

		if (LFO->Gate[0] > 0)
		{
			LFO->Gate[0]--;
		}
		
		if (LFO->Gate[1] > 0) 
		{
			LFO->Gate[1]--;
		}

		if (LFO->EnvelopeState != Wobbler2_IDLE)
		{
			uint32_t A = 0;
			uint32_t R = LFORange(128, 2000) >> 12;
			if (LFO->Mod < 128)
			{
				R = 1 + (LFORange(LFO->Mod, 2000) >> 12);
			}
			else
			{
				A = 1 + (LFORange(LFO->Mod - (128<<8), 2000) >> 12);
			}
			if (LFO->EnvelopeState == Wobbler2_ATTACK)
			{
				if (A == 0)
				{
					LFO->EnvelopeState = Wobbler2_RELEASE;
					LFO->EnvelopeVal = 1 << 24;
				}
				else
				{
					LFO->EnvelopeVal += ((1 << 24) - 1) / A;
					if (LFO->EnvelopeVal >= 1 << 24)
					{
						LFO->EnvelopeVal = 1 << 24;
						LFO->EnvelopeState = Wobbler2_RELEASE;
					}
				}
			}
			else
			{
				LFO->EnvelopeVal -= ((1 << 24) - 1) / R;
				if (LFO->EnvelopeVal <= 0)
				{
					LFO->EnvelopeState = Wobbler2_IDLE;
					LFO->EnvelopeVal = 0;
				}
			}
		}


		uint32_t DP = LFORange2(LFO->Speed);;
		LFO->Phase1 += DP;

		uint32_t DP2 = LFO->Phasing * 0x100000;
		//DP2 <<= 24;
		LFO->Phase2 = LFO->Phase1 + DP2;

		for (int i = 0; i < 9; i++)
		{
			LFO->Led[0][i] = 0;
			LFO->Led[1][i] = 0;
		}

		if (LFO->Phase1 < LFO->OldPhase1)
		{
			LFO->Gate[1] = Wobbler2_GATECOUNTDOWN;
			if (LFO->PhasedCountdown > 0)
			{
				LFO->Gate[0] = Wobbler2_GATECOUNTDOWN;
			}
			LFO->PhasedCountdown = LFO->Phasing << 24;
		}

		uint32_t last = LFO->PhasedCountdown;
		LFO->PhasedCountdown -= __min(DP, LFO->PhasedCountdown);
		if (LFO->PhasedCountdown == 0 && last != 0)
		{
			LFO->Gate[0] = Wobbler2_GATECOUNTDOWN;
		}

		LFO->OldPhase1 = LFO->Phase1;
		LFO->OldPhase2 = LFO->Phase2;

		int32_t O[4];
		int32_t P[4];
		Wobbler2_DoublePendulum(&LFO->Pendulum, 0.05f);
		O[0] = BasicShapes(LFO->Phase1, LFO->Mod>>8);
		O[1] = Wobbler2_Twang(LFO, LFO->Phase1);
		O[2] = Wobbler2_SampleHold(&LFO->SNH[0], LFO, LFO->Phase1, LFO->Mod);

		O[2] = LFO->Pendulum.A;

		P[0] = BasicShapes(LFO->Phase2, LFO->Mod>>8);
		P[1] = Wobbler2_Twang(LFO, LFO->Phase2);
		P[2] = Wobbler2_SampleHold(&LFO->SNH[1], LFO, LFO->Phase2, LFO->Mod);
		P[2] = LFO->Pendulum.B;


		struct SteppedResult_t sr;
		GetSteppedResult(LFO->Shape, 2, &sr);

		LFO->Output = GetInterpolatedResultInt(O, &sr) /(0xffff*4);
		 LFO->OutputPhased = GetInterpolatedResultInt(P, &sr) /(0xffff*4);


		LFO->Output += 2048;// + (2540 - 2048);
		LFO->OutputPhased += 2048;// +(2540 - 2048);

		if (LFO->Output > 4095) LFO->Output = 4095; else if (LFO->Output < 0) LFO->Output = 0;
		if (LFO->OutputPhased > 4095) LFO->OutputPhased = 4095; else if (LFO->OutputPhased < 0) LFO->OutputPhased = 0;
		//if (LFO->Output > 1) LFO->Output = 1; else if (LFO->Output < -1) LFO->Output = -1;

		int32_t LedIdxB = (LFO->Output * 8);
		int iLedIdxB = LedIdxB >> 12;
		int IdxB = ((LedIdxB - (iLedIdxB << 12))) >> 4;

		LFO->Led[0][(iLedIdxB + 9) % 9] = 255 - IdxB;
		LFO->Led[0][(iLedIdxB + 10) % 9] = IdxB;

		int32_t LedIdxA = (LFO->OutputPhased * 8);
		int iLedIdxA = LedIdxA >> 12;
		int IdxA = ((LedIdxA - (iLedIdxA << 12))) >> 4;

		LFO->Led[1][(iLedIdxA + 9) % 9] = 255 - IdxA;
		LFO->Led[1][(iLedIdxA + 10) % 9] = IdxA;
		for(int i = 0;i<3;i++){LFO->ModeLed[i] = 0;};
		LFO->ModeLed[sr.index] = sr.fractional;
		LFO->ModeLed[sr.index+1] = 255-sr.fractional;

		return LFO->Output;
	}

	int32_t GetInterpolatedResultInt(int32_t *table, SteppedResult_t *inp)
	{
		unsigned char F = inp->fractional;
		unsigned char IF =  ~inp->fractional;

		return ((table[inp->index]/256) * IF)  + ((table[inp->index + 1]/256) * F);
	}

	void GetSteppedResult(uint16_t param, uint8_t steps, SteppedResult_t *out)
	{
		//max(floor(x + 0.25), (x + 0.25 - floor(x + 0.25)) * 2 + floor(x + 0.25) - 1)

		uint32_t X = (param * steps)/256;
		X += 64;

		int FloorX = X &0xffffff00;
		int Aside = FloorX;
		int Bside = (X - (FloorX)) * 2 + (FloorX)-256;
		int M = (Aside > Bside) ? Aside : Bside;

		out->index = M >> 8;
		out->fractional = M & 0xff;

	}


#ifdef __cplusplus
}
#endif