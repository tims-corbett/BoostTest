function [TransitionMatrix ScaleMatrix ShapeMatrix] = getWblMatrix(inputData)


[ scale.omega shape.omega ]= GetWblParams( inputData.omega,  inputData.cov_omega  );
[ scale.mu shape.mu ]= GetWblParams(inputData.mu,  inputData.cov_mu  );
[ scale.rho_F shape.rho_F ]= GetWblParams(inputData.rho_F,  inputData.cov_rho_F  );
[ scale.lambda_F shape.lambda_F ]= GetWblParams(inputData.lambda_F,  inputData.cov_lambda_F  );
[ scale.rho_L shape.rho_L ]= GetWblParams(inputData.rho_L , inputData.cov_rho_L  );
[ scale.phi shape.phi]= GetWblParams(inputData.phi,  inputData.cov_phi  );


TransitionMatrix = [     
    %S                      F               L               R
    0               inputData.phi          0                   0               %S
    inputData.omega           0            inputData.lambda_F    inputData.rho_F     %F
    inputData.mu       0                   0                  inputData.rho_L     %L
    0               0                   0                   0               %R            
    ];


ScaleMatrix = [     
    %S                      F               L               R
    0               scale.phi          0                   0               %S
    scale.omega           0            scale.lambda_F    scale.rho_F     %F
    scale.mu       0                   0                  scale.rho_L     %L
    0               0                   0                   0               %R            
    ];


ShapeMatrix = [     
    %S                      F               L               R
    1               shape.phi          1                   1               %S
    shape.omega           1            shape.lambda_F    shape.rho_F     %F
    shape.mu       1                   1                  shape.rho_L     %L
    1               1                   1                   1               %R            
    ];
