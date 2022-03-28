float init = 1.000f;

float run_fpu() {
    init = (0.2232f * init) + init;
    return init;
}