namespace ResourceCompiler
{
    class Program
    {
        static void Main(string[] args)
        {
            foreach (var file in System.IO.Directory.GetFiles(@"..\..\..\Resources", "*.png"))
                GlyphCompiler.Compile(file);
        }
    }
}
