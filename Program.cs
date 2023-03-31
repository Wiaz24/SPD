using System;
using System.Collections.Generic;
using System.Linq;
using static System.Runtime.InteropServices.JavaScript.JSType;

public struct WTask
{
    public int P { get; set; }
    public int W { get; set; }
    public int D { get; set; }
    public int ID { get; set; }
    public WTask(int p, int w, int d, int id) : this()
    {
        P = p; W = w; D = d; ID = id;
    }
}

namespace Witi
{
    
    public class Program
    {
        public static List<WTask> allTasks = new List<WTask>();
        public static bool IsBitSet(int number, int bit)
        {
            int mask = 1 << bit;
            return (number & mask) != 0;
        }
        public static void SetBitTo(ref int number, int bit, bool value) 
        {
            if (value == true)   number |= 1 << bit;
            else                 number &= ~(1 << bit);
        }
        public static int CalcSumTime(int comb, List<WTask> tasks)
        {
            int sum = 0;
            for (int i = 0; i < tasks.Count; i++)
            {
                if (IsBitSet(comb, i) == true) sum += tasks[i].P;
            }
            return sum;
        }
        public static void CalcOptim(int comb, List<int> optValues, List<WTask> allTasks)
        {
            if (comb == 0) optValues[comb] = 0;
            else
            {
                int sum = CalcSumTime(comb, allTasks);
                List<int> mins = new List<int>();
                for (int i = 0; i < allTasks.Count; i++)    //iteracja po wszystkich bitach danej kombinacji
                {
                    int tmpComb = comb; //tmpComb będzie zawsze bez jednego zadania
                    SetBitTo(ref tmpComb, i, false);
                    if (tmpComb != comb)    //jeśli nastąpiła zmiana
                    {
                        int Fval = optValues[tmpComb] + allTasks[i].W * (sum - allTasks[i].D);
                        if (Fval < 0) Fval = 0;
                        mins.Add(Fval);
                    }
                }
                optValues[comb] = mins.Min();
                
            }
            Console.WriteLine("kombinacja: " + comb + " (" + Convert.ToString(comb, 2).PadLeft(4, '0') + ") wartosc: " + optValues[comb]);
        }

        public static void Main(string[] args)
        {
            //FileStream F = new FileStream("data.txt", FileMode.Open, FileAccess.Read, FileShare.Read);
            var path = "C:\\Users\\user\\Desktop\\Semestr6\\Csharp\\ConsoleApp1\\ConsoleApp1\\data.txt";
            StreamReader reader = new StreamReader(path);
            string? line;
            while ((line = reader.ReadLine()) != "data.11:") { }
            int N = int.Parse(reader.ReadLine() ?? "0");
            Console.WriteLine(N);

            for (int i = 0; i < N; i++)
            {
                line = reader.ReadLine();
                string[] words = line.Split(new char[] { ',', ' ' }, StringSplitOptions.RemoveEmptyEntries);
                WTask tmp = new WTask(int.Parse(words[0]), int.Parse(words[1]), int.Parse(words[2]), i);
                allTasks.Add(tmp);
            }
            foreach (WTask item in allTasks)
            {
                Console.WriteLine("Task " + item.ID + " - " + $"{item.P} : {item.W} : {item.D}");
            }

            List<int> optValues = Enumerable.Repeat(999, (int)Math.Pow(2,N)).ToList();
            for (int i = 0; i < optValues.Count;i++)
            {
                CalcOptim(i, optValues, allTasks);
            }
            Console.WriteLine(optValues.Last());
        }
    }
}
