import regression from 'regression';

// Define your data as [x, y] coordinates
const data = [[1, 2], [2, 3.9], [3, 6.1], [4, 8]];

// Run the linear regression
const result = regression.linear(data);

console.log(result.equation);  // [slope, y-intercept] -> e.g., [2, 0]
console.log(result.r2);        // Coefficient of determination (R²)
console.log(result.predict(5)); // Predicts the [x, y] point for x = 5