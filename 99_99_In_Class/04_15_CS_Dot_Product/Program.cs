using System;
using System.Collections.Generic;
class Program
{
    static async Task Main(string[] args)
    {
        // Generate two sample vectors of length 10,000
        var vectorA = GenerateVector(10000, 1.0); // all elements = 1.0
        var vectorB = GenerateVector(10000, 2.0); // all elements = 2.0
        var dotProd = ComputeDotProduct(vectorA, vectorB);
        double dotProduct = await dotProd;
        Console.WriteLine($"Dot product: {dotProduct}");
    }
    static List<double> GenerateVector(int length, double value)
    {
        var vec = new List<double>(length);
        for (int i = 0; i < length; i++)
        {
            vec.Add(value);
        }
        return vec;
    }
    static async Task<double> ComputeDotProduct(List<double> vec1, List<double> vec2)
    {
        if (vec1.Count != vec2.Count)
            throw new ArgumentException("Vectors must be the same length.");
        double sum = 0.0;
        await Task.Run(() =>
        {
            for (int i = 0; i < vec1.Count; i++)
                sum += vec1[i] * vec2[i];
        });
       
        return sum;
    }
}