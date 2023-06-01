using System.Runtime.InteropServices;

namespace LibSample;

public static class LibSample
{
    const string GREETING = "Hello there";

    [UnmanagedCallersOnly(EntryPoint = "add")]
    public static int Add(int a, int b) => a + b;

    [UnmanagedCallersOnly(EntryPoint = "subtract")]
    public static int Subtract(int a, int b) => a - b;

    [UnmanagedCallersOnly(EntryPoint = "multiply")]
    public static int Multiply(int a, int b) => a * b;

    // UnmanagedCallersOnly methods only accept primitive arguments. 
    // The primitive arguments have to be marshalled manually if necessary.
    [UnmanagedCallersOnly(EntryPoint = "greet")]
    public static IntPtr Greet(IntPtr namePointer) 
    {
        // Parse string from the passed pointer
        string name = Marshal.PtrToStringAnsi(namePointer) ?? "friend";

        // Concatenate strings 
        string greeting = $"{GREETING}, {name}";

        // Assign pointer of the concatenated string to sumPointer
        IntPtr sumPointer = Marshal.StringToHGlobalAnsi(greeting);

        // Return pointer
        return sumPointer;
    }
}