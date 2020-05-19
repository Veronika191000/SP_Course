using System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

// Документацию по шаблону элемента "Пустая страница" см. по адресу https://go.microsoft.com/fwlink/?LinkId=234238

namespace SystemProgrammingLab6
{
    /// <summary>
    /// Пустая страница, которую можно использовать саму по себе или для перехода внутри фрейма.
    /// </summary>
    public enum Operations
    {
        Add = 0,
        Sub = 1,
        Mul = 2,
        Div = 3,
        Sin = 4,
        Cos = 5,
        Tan = 6,
        Sqrt = 7,
        Log = 8,
        Pow = 9
    }
    public sealed partial class Calculator : Page
    {
        private Operations _selectedOperation;
        public Calculator()
        {
            this.InitializeComponent();
        }
        private void CalculateButton_Click(object sender, RoutedEventArgs e)
        {
            double x1, x2;

            Result.Text = "";

            if (!Double.TryParse(A.Text, out x1))
            {
                Result.Text = "Операнд х1 не число!";
                return;
            }

            if (!Double.TryParse(B.Text, out x2))
            {
                Result.Text = "Операнд х2 не число!";
                return;
            }

            switch (_selectedOperation)
            {
                case Operations.Add: Result.Text = (x1 + x2).ToString(); break;
                case Operations.Sub: Result.Text = (x1 - x2).ToString(); break;
                case Operations.Mul: Result.Text = (x1 * x2).ToString(); break;
                case Operations.Div: Result.Text = (x2 == 0 ? "Нельзя делить на ноль" : (x1 / x2).ToString()); break;
                case Operations.Sin: Result.Text = (Math.Sin(x1)).ToString(); break;
                case Operations.Cos: Result.Text = (Math.Cos(x1)).ToString(); break;
                case Operations.Tan: Result.Text = (Math.Tan(x1)).ToString(); break;
                case Operations.Sqrt: Result.Text = (x1 < 0 ? "Значение не может быть меньше нуля!" : (Math.Sqrt(x1)).ToString()); break;
                case Operations.Log: Result.Text = (Math.Log(x1)).ToString(); break;
                case Operations.Pow: Result.Text = (Math.Pow(x1, x2)).ToString(); break;
            }
        }
        private void RadioButton_Checked(object sender, RoutedEventArgs e)
        {
            RadioButton pressed = (RadioButton)sender;
            if (pressed.Tag == null)return;
            _selectedOperation = (Operations)pressed.Tag;
        }
    }
}

